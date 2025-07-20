#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "consistent_hashing.h"

#define NUM_NODES 10
#define NUM_KEYS 1000000
#define REPLICAS 100 // Number of virtual nodes per real node
#define REMAP_KEYS 10000 // Number of keys to test for remapping

int main() {
    // --- Key Distribution Experiment ---
    HashRing* hr_dist = create_hash_ring(REPLICAS);
    if (!hr_dist) {
        fprintf(stderr, "Failed to create hash ring for distribution experiment\n");
        return 1;
    }

    char node_name_dist[20];
    int counts_dist[NUM_NODES] = {0};

    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_name_dist, "node%d", i);
        add_node(hr_dist, node_name_dist);
    }

    char key_dist[20];
    for (int i = 0; i < NUM_KEYS; i++) {
        sprintf(key_dist, "key%d", i);
        const char* node = get_node(hr_dist, key_dist);
        if (node) {
            for (int j = 0; j < NUM_NODES; j++) {
                sprintf(node_name_dist, "node%d", j);
                if (strcmp(node, node_name_dist) == 0) {
                    counts_dist[j]++;
                    break;
                }
            }
        }
    }

    // Print distribution results to stdout
    printf("KARGER_DIST_START\n");
    printf("Node,Keys,Algorithm\n");
    for (int i = 0; i < NUM_NODES; i++) {
        printf("node%d,%d,karger_c\n", i, counts_dist[i]);
    }
    printf("KARGER_DIST_END\n");
    destroy_hash_ring(hr_dist);

    // --- Remapping Experiment ---
    HashRing* hr_remap = create_hash_ring(REPLICAS);
    if (!hr_remap) {
        fprintf(stderr, "Failed to create hash ring for remapping experiment\n");
        return 1;
    }

    char initial_node[REMAP_KEYS][20];
    char remap_key[20];
    int remapped_on_remove = 0;
    int remapped_on_add = 0;

    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_name_dist, "node%d", i); // Reusing node_name_dist for convenience
        add_node(hr_remap, node_name_dist);
    }

    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* node = get_node(hr_remap, remap_key);
        if (node) {
            strcpy(initial_node[i], node);
        } else {
            strcpy(initial_node[i], "NULL");
        }
    }

    // Remove one node (e.g., "node9")
    remove_node(hr_remap, "node9");

    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* new_node = get_node(hr_remap, remap_key);
        if (!new_node || strcmp(initial_node[i], new_node) != 0) {
            remapped_on_remove++;
        }
    }

    // Add the node back
    add_node(hr_remap, "node9");

    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* new_node = get_node(hr_remap, remap_key);
        if (!new_node || strcmp(initial_node[i], new_node) != 0) {
            remapped_on_add++;
        }
    }

    // Print remapping results to stdout
    printf("KARGER_REMAP_START\n");
    printf("Operation,RemappedKeys,TotalKeys,Algorithm\n");
    printf("Remove, %d, %d, karger_c\n", remapped_on_remove, REMAP_KEYS);
    printf("Add, %d, %d, karger_c\n", remapped_on_add, REMAP_KEYS);
    printf("KARGER_REMAP_END\n");
    destroy_hash_ring(hr_remap);

    return 0;
}