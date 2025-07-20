#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "dx_hash.h"

#define NUM_NODES 10
#define NUM_KEYS 1000000
#define HASH_SIZE 20 // Must be >= NUM_NODES
#define REMAP_KEYS 10000 // Number of keys to test for remapping

int main() {
    char key_dist[20]; // Moved declaration to top
    char remap_key[20]; // Moved declaration to top

    // --- Key Distribution Experiment ---
    dx_hash_t* h_dist = dx_hash_new(HASH_SIZE);
    char node_name_dist[20];
    int counts_dist[NUM_NODES] = {0};

    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_name_dist, "node%d", i);
        dx_hash_add(h_dist, node_name_dist);
    }

    for (int i = 0; i < NUM_KEYS; i++) {
        sprintf(key_dist, "key%d", i);
        const char* node = dx_hash_get(h_dist, key_dist);
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
    printf("DX_DIST_START\n");
    printf("Node,Keys,Algorithm\n");
    for (int i = 0; i < NUM_NODES; i++) {
        printf("node%d,%d,dx_c\n", i, counts_dist[i]);
    }
    printf("DX_DIST_END\n");
    dx_hash_free(h_dist);

    // --- Remapping Experiment ---
    dx_hash_t* h_remap = dx_hash_new(HASH_SIZE);
    char initial_node[REMAP_KEYS][20];
    int remapped_on_remove = 0;
    int remapped_on_add = 0;

    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_name_dist, "node%d", i);
        dx_hash_add(h_remap, node_name_dist);
    }

    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* node = dx_hash_get(h_remap, remap_key);
        if (node) {
            strcpy(initial_node[i], node);
        } else {
            strcpy(initial_node[i], "NULL");
        }
    }

    dx_hash_remove(h_remap, "node9");

    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* new_node = dx_hash_get(h_remap, remap_key);
        if (!new_node || strcmp(initial_node[i], new_node) != 0) {
            remapped_on_remove++;
        }
    }

    dx_hash_add(h_remap, "node9");

    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* new_node = dx_hash_get(h_remap, remap_key);
        if (!new_node || strcmp(initial_node[i], new_node) != 0) {
            remapped_on_add++;
        }
    }

    // Print remapping results to stdout
    printf("DX_REMAP_START\n");
    printf("Operation,RemappedKeys,TotalKeys,Algorithm\n");
    printf("Remove, %d, %d, dx_c\n", remapped_on_remove, REMAP_KEYS);
    printf("Add, %d, %d, dx_c\n", remapped_on_add, REMAP_KEYS);
    printf("DX_REMAP_END\n");
    dx_hash_free(h_remap);

    return 0;
}