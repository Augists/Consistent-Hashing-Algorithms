#include "multi_probe_hashing.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Simple CRC32 implementation (for demonstration purposes)
// In a real application, use a robust CRC32 library.
static uint32_t crc32_checksum(const char *s) {
    uint32_t crc = 0xFFFFFFFF; // Initial value
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        crc ^= s[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320; // CRC32 polynomial
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc; // Final XOR
}

// Helper to find insertion point for a hash in the sorted nodes array
static int find_insert_pos_multi_probe(MultiProbeHashRing* hr, uint32_t hash) {
    int low = 0;
    int high = hr->num_nodes - 1;
    int pos = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (hr->nodes[mid].hash == hash) {
            return mid; // Exact match, though we usually insert unique hashes
        } else if (hr->nodes[mid].hash < hash) {
            low = mid + 1;
            pos = low;
        } else {
            high = mid - 1;
            pos = mid;
        }
    }
    return pos;
}

MultiProbeHashRing* create_multi_probe_hash_ring(int replicas, int probes) {
    MultiProbeHashRing* hr = (MultiProbeHashRing*)malloc(sizeof(MultiProbeHashRing));
    if (!hr) return NULL;

    hr->replicas = replicas;
    hr->probes = probes;
    hr->num_nodes = 0;
    hr->capacity = 16; // Initial capacity
    hr->nodes = (MultiProbeNode*)malloc(sizeof(MultiProbeNode) * hr->capacity);

    if (!hr->nodes) {
        destroy_multi_probe_hash_ring(hr);
        return NULL;
    }
    return hr;
}

void destroy_multi_probe_hash_ring(MultiProbeHashRing* hr) {
    if (!hr) return;
    if (hr->nodes) free(hr->nodes);
    free(hr);
}

void add_multi_probe_node(MultiProbeHashRing* hr, const char* node_name) {
    if (!hr) return;

    // Resize if necessary
    if (hr->num_nodes + hr->replicas > hr->capacity) {
        hr->capacity = (hr->num_nodes + hr->replicas) * 2; // Double capacity
        hr->nodes = (MultiProbeNode*)realloc(hr->nodes, sizeof(MultiProbeNode) * hr->capacity);
        if (!hr->nodes) {
            fprintf(stderr, "Failed to reallocate memory for Multi-Probe hash ring.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < hr->replicas; i++) {
        char virtual_node_name[256];
        snprintf(virtual_node_name, sizeof(virtual_node_name), "%s-%d", node_name, i);
        uint32_t hash = crc32_checksum(virtual_node_name);

        int pos = find_insert_pos_multi_probe(hr, hash);

        // Shift elements to make space for the new node
        for (int j = hr->num_nodes; j > pos; j--) {
            hr->nodes[j] = hr->nodes[j-1];
        }
        hr->nodes[pos].hash = hash;
        hr->nodes[pos].node_name = (char*)node_name; // Store pointer to original node name
        hr->num_nodes++;
    }
}

void remove_multi_probe_node(MultiProbeHashRing* hr, const char* node_name) {
    if (!hr) return;

    int original_num_nodes = hr->num_nodes;
    int write_idx = 0;

    for (int i = 0; i < original_num_nodes; i++) {
        if (strcmp(hr->nodes[i].node_name, node_name) != 0) {
            hr->nodes[write_idx] = hr->nodes[i];
            write_idx++;
        }
    }
    hr->num_nodes = write_idx;

    // Re-sort after removal (optional, but good for consistency if not all nodes are removed)
    // For simplicity, we assume the remaining nodes are still sorted relative to each other
    // if the original list was sorted. A full sort would be needed if order is critical.
}

const char* get_multi_probe_node(MultiProbeHashRing* hr, const char* key) {
    if (!hr || hr->num_nodes == 0) {
        return NULL;
    }

    const char* best_node = NULL;
    uint32_t min_distance = 0xFFFFFFFF; // Max uint32 value

    for (int p = 0; p < hr->probes; p++) {
        char probe_key[256];
        snprintf(probe_key, sizeof(probe_key), "%s_probe_%d", key, p);
        uint32_t probe_hash = crc32_checksum(probe_key);

        // Find the first node with a hash greater than or equal to the probe hash
        int low = 0;
        int high = hr->num_nodes - 1;
        int idx = 0; // Default to first node if no greater hash is found

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (hr->nodes[mid].hash >= probe_hash) {
                idx = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        uint32_t current_node_hash = hr->nodes[idx].hash;
        const char* current_node_name = hr->nodes[idx].node_name;

        // Calculate distance (clockwise distance on the ring)
        uint32_t distance;
        if (current_node_hash >= probe_hash) {
            distance = current_node_hash - probe_hash;
        } else {
            distance = 0xFFFFFFFF - probe_hash + current_node_hash + 1;
        }

        if (distance < min_distance) {
            min_distance = distance;
            best_node = current_node_name;
        }
    }
    return best_node;
}
