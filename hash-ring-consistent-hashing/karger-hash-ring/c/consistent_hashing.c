#include "consistent_hashing.h"
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
static int find_insert_pos(HashRing* hr, uint32_t hash) {
    int low = 0;
    int high = hr->num_nodes - 1;
    int pos = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (hr->nodes[mid] == hash) {
            return mid; // Exact match, though we usually insert unique hashes
        } else if (hr->nodes[mid] < hash) {
            low = mid + 1;
            pos = low;
        } else {
            high = mid - 1;
            pos = mid;
        }
    }
    return pos;
}

HashRing* create_hash_ring(int replicas) {
    HashRing* hr = (HashRing*)malloc(sizeof(HashRing));
    if (!hr) return NULL;

    hr->replicas = replicas;
    hr->num_nodes = 0;
    hr->capacity = 16; // Initial capacity
    hr->nodes = (uint32_t*)malloc(sizeof(uint32_t) * hr->capacity);
    hr->node_names = (char**)malloc(sizeof(char*) * hr->capacity);

    if (!hr->nodes || !hr->node_names) {
        destroy_hash_ring(hr);
        return NULL;
    }
    return hr;
}

void destroy_hash_ring(HashRing* hr) {
    if (!hr) return;
    if (hr->nodes) free(hr->nodes);
    if (hr->node_names) {
        // Free individual node names if they were dynamically allocated
        // For this simple implementation, we assume node_names are static strings
        // or managed externally. If they were strdup'd, they'd need freeing here.
        free(hr->node_names);
    }
    free(hr);
}

void add_node(HashRing* hr, const char* node_name) {
    if (!hr) return;

    // Resize if necessary
    if (hr->num_nodes + hr->replicas > hr->capacity) {
        hr->capacity *= 2;
        hr->nodes = (uint32_t*)realloc(hr->nodes, sizeof(uint32_t) * hr->capacity);
        hr->node_names = (char**)realloc(hr->node_names, sizeof(char*) * hr->capacity);
        if (!hr->nodes || !hr->node_names) {
            fprintf(stderr, "Failed to reallocate memory for hash ring.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < hr->replicas; i++) {
        char virtual_node_name[256];
        snprintf(virtual_node_name, sizeof(virtual_node_name), "%s-%d", node_name, i);
        uint32_t hash = crc32_checksum(virtual_node_name);

        int pos = find_insert_pos(hr, hash);

        // Shift elements to make space for the new node
        for (int j = hr->num_nodes; j > pos; j--) {
            hr->nodes[j] = hr->nodes[j-1];
            hr->node_names[j] = hr->node_names[j-1];
        }
        hr->nodes[pos] = hash;
        hr->node_names[pos] = (char*)node_name; // Store pointer to original node name
        hr->num_nodes++;
    }
}

void remove_node(HashRing* hr, const char* node_name) {
    if (!hr) return;

    for (int i = 0; i < hr->replicas; i++) {
        char virtual_node_name[256];
        snprintf(virtual_node_name, sizeof(virtual_node_name), "%s-%d", node_name, i);
        uint32_t hash_to_remove = crc32_checksum(virtual_node_name);

        int found_idx = -1;
        for (int j = 0; j < hr->num_nodes; j++) {
            if (hr->nodes[j] == hash_to_remove) {
                found_idx = j;
                break;
            }
        }

        if (found_idx != -1) {
            // Shift elements to fill the gap
            for (int j = found_idx; j < hr->num_nodes - 1; j++) {
                hr->nodes[j] = hr->nodes[j+1];
                hr->node_names[j] = hr->node_names[j+1];
            }
            hr->num_nodes--;
        }
    }
}

const char* get_node(HashRing* hr, const char* key) {
    if (!hr || hr->num_nodes == 0) {
        return NULL;
    }

    uint32_t key_hash = crc32_checksum(key);

    // Find the first node with a hash greater than or equal to the key's hash
    int idx = find_insert_pos(hr, key_hash);

    // If no such node is found, wrap around to the first node
    if (idx == hr->num_nodes) {
        idx = 0;
    }
    return hr->node_names[idx];
}
