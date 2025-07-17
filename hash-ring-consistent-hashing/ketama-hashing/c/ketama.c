#include "ketama.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Simple MD5 implementation (for demonstration purposes)
// In a real application, use a robust MD5 library.
// This is a placeholder and not a full MD5 implementation.
// It's just enough to generate a 16-byte hash for demonstration.
static void simple_md5(const char *s, unsigned char *digest) {
    // This is a highly simplified placeholder for MD5.
    // DO NOT use this for any security-sensitive applications.
    // A real MD5 implementation is complex.
    // For demonstration, we'll just use a basic hash based on string length and sum.
    size_t len = strlen(s);
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += s[i];
    }

    // Fill digest with some derived values
    digest[0] = (unsigned char)(sum & 0xFF);
    digest[1] = (unsigned char)((sum >> 8) & 0xFF);
    digest[2] = (unsigned char)((sum >> 16) & 0xFF);
    digest[3] = (unsigned char)((sum >> 24) & 0xFF);
    // Repeat for 16 bytes, using different shifts/operations for variety
    digest[4] = (unsigned char)((sum * 3) & 0xFF);
    digest[5] = (unsigned char)(((sum * 3) >> 8) & 0xFF);
    digest[6] = (unsigned char)(((sum * 3) >> 16) & 0xFF);
    digest[7] = (unsigned char)(((sum * 3) >> 24) & 0xFF);
    digest[8] = (unsigned char)((sum + len) & 0xFF);
    digest[9] = (unsigned char)(((sum + len) >> 8) & 0xFF);
    digest[10] = (unsigned char)(((sum + len) >> 16) & 0xFF);
    digest[11] = (unsigned char)(((sum + len) >> 24) & 0xFF);
    digest[12] = (unsigned char)((sum ^ len) & 0xFF);
    digest[13] = (unsigned char)(((sum ^ len) >> 8) & 0xFF);
    digest[14] = (unsigned char)(((sum ^ len) >> 16) & 0xFF);
    digest[15] = (unsigned char)(((sum ^ len) >> 24) & 0xFF);
}

// Helper to find insertion point for a hash in the sorted nodes array
static int find_insert_pos_ketama(KetamaRing* kr, uint32_t hash) {
    int low = 0;
    int high = kr->num_nodes - 1;
    int pos = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (kr->nodes[mid].hash == hash) {
            return mid; // Exact match, though we usually insert unique hashes
        } else if (kr->nodes[mid].hash < hash) {
            low = mid + 1;
            pos = low;
        } else {
            high = mid - 1;
            pos = mid;
        }
    }
    return pos;
}

KetamaRing* create_ketama_ring(int num_virtual_nodes) {
    KetamaRing* kr = (KetamaRing*)malloc(sizeof(KetamaRing));
    if (!kr) return NULL;

    kr->num_virtual_nodes = num_virtual_nodes;
    kr->num_nodes = 0;
    kr->capacity = 16; // Initial capacity
    kr->nodes = (KetamaNode*)malloc(sizeof(KetamaNode) * kr->capacity);

    if (!kr->nodes) {
        destroy_ketama_ring(kr);
        return NULL;
    }
    return kr;
}

void destroy_ketama_ring(KetamaRing* kr) {
    if (!kr) return;
    if (kr->nodes) free(kr->nodes);
    free(kr);
}

void add_ketama_node(KetamaRing* kr, const char* node_name) {
    if (!kr) return;

    // Each MD5 hash produces 16 bytes, which can be interpreted as 4 32-bit integers.
    // So, for num_virtual_nodes, we need to generate num_virtual_nodes / 4 MD5 hashes.
    int hashes_per_node = kr->num_virtual_nodes / 4;

    // Resize if necessary
    if (kr->num_nodes + kr->num_virtual_nodes > kr->capacity) {
        kr->capacity = (kr->num_nodes + kr->num_virtual_nodes) * 2; // Double capacity
        kr->nodes = (KetamaNode*)realloc(kr->nodes, sizeof(KetamaNode) * kr->capacity);
        if (!kr->nodes) {
            fprintf(stderr, "Failed to reallocate memory for Ketama ring.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < hashes_per_node; i++) {
        char virtual_node_seed[256];
        snprintf(virtual_node_seed, sizeof(virtual_node_seed), "%s-%d", node_name, i);
        unsigned char digest[16];
        simple_md5(virtual_node_seed, digest);

        for (int j = 0; j < 4; j++) {
            uint32_t hash = (uint32_t)digest[j*4] | 
                            (uint32_t)digest[j*4+1] << 8 | 
                            (uint32_t)digest[j*4+2] << 16 | 
                            (uint32_t)digest[j*4+3] << 24;

            int pos = find_insert_pos_ketama(kr, hash);

            // Shift elements to make space for the new node
            for (int k = kr->num_nodes; k > pos; k--) {
                kr->nodes[k] = kr->nodes[k-1];
            }
            kr->nodes[pos].hash = hash;
            kr->nodes[pos].node_name = (char*)node_name; // Store pointer to original node name
            kr->num_nodes++;
        }
    }
}

void remove_ketama_node(KetamaRing* kr, const char* node_name) {
    if (!kr) return;

    int original_num_nodes = kr->num_nodes;
    int write_idx = 0;

    for (int i = 0; i < original_num_nodes; i++) {
        if (strcmp(kr->nodes[i].node_name, node_name) != 0) {
            kr->nodes[write_idx] = kr->nodes[i];
            write_idx++;
        }
    }
    kr->num_nodes = write_idx;

    // Re-sort after removal (optional, but good for consistency if not all nodes are removed)
    // For simplicity, we assume the remaining nodes are still sorted relative to each other
    // if the original list was sorted. A full sort would be needed if order is critical.
    // For this demo, we rely on the fact that we only remove specific node's virtual points.
}

const char* get_ketama_node(KetamaRing* kr, const char* key) {
    if (!kr || kr->num_nodes == 0) {
        return NULL;
    }

    unsigned char digest[16];
    simple_md5(key, digest);
    uint32_t key_hash = (uint32_t)digest[0] | 
                        (uint32_t)digest[1] << 8 | 
                        (uint32_t)digest[2] << 16 | 
                        (uint32_t)digest[3] << 24;

    // Find the first node with a hash greater than or equal to the key's hash
    int low = 0;
    int high = kr->num_nodes - 1;
    int idx = 0; // Default to first node if no greater hash is found

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (kr->nodes[mid].hash >= key_hash) {
            idx = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    return kr->nodes[idx].node_name;
}
