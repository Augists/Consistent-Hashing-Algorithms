#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // Added for strcmp
#include "jump_hash.h"

#define NUM_NODES 10
#define NUM_KEYS 1000000
#define REMAP_KEYS 10000 // Number of keys to test for remapping

int main() {
    // --- Key Distribution Experiment ---
    int counts_dist[NUM_NODES] = {0};

    for (uint64_t i = 0; i < NUM_KEYS; i++) {
        int32_t bucket = jump_consistent_hash(i, NUM_NODES);
        if (bucket >= 0 && bucket < NUM_NODES) {
            counts_dist[bucket]++;
        }
    }

    // Print distribution results to stdout
    printf("JUMP_DIST_START\n");
    printf("Node,Keys,Algorithm\n");
    for (int i = 0; i < NUM_NODES; i++) {
        printf("node%d,%d,jump_c\n", i, counts_dist[i]);
    }
    printf("JUMP_DIST_END\n");

    // --- Remapping Experiment ---
    int32_t initial_bucket[REMAP_KEYS];
    int remapped_on_remove = 0;
    int remapped_on_add = 0;

    // Store initial mappings
    for (uint64_t i = 0; i < REMAP_KEYS; i++) {
        initial_bucket[i] = jump_consistent_hash(i, NUM_NODES);
    }

    // Remove one node (simulate by reducing num_nodes)
    // Jump Consistent Hash doesn't have explicit node removal,
    // but we can simulate by changing the total number of buckets.
    // Let's remove "node9" by reducing total buckets to 9.
    int32_t num_buckets_after_remove = NUM_NODES - 1;

    // Count remappings after removal
    for (uint64_t i = 0; i < REMAP_KEYS; i++) {
        int32_t new_bucket = jump_consistent_hash(i, num_buckets_after_remove);
        if (initial_bucket[i] != new_bucket) {
            remapped_on_remove++;
        }
    }

    // Add the node back (simulate by restoring num_nodes)
    int32_t num_buckets_after_add = NUM_NODES;

    // Count remappings after addition
    for (uint64_t i = 0; i < REMAP_KEYS; i++) {
        int32_t new_bucket = jump_consistent_hash(i, num_buckets_after_add);
        if (initial_bucket[i] != new_bucket) {
            remapped_on_add++;
        }
    }

    // Print remapping results to stdout
    printf("JUMP_REMAP_START\n");
    printf("Operation,RemappedKeys,TotalKeys,Algorithm\n");
    printf("Remove, %d, %d, jump_c\n", remapped_on_remove, REMAP_KEYS);
    printf("Add, %d, %d, jump_c\n", remapped_on_add, REMAP_KEYS);
    printf("JUMP_REMAP_END\n");

    return 0;
}