#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "anchor_hash.h"

#define NUM_NODES 10
#define NUM_KEYS 1000000
#define REMAP_KEYS 10000 // Number of keys to test for remapping

int main() {
    // --- Key Distribution Experiment (Existing) ---
    anchor_hash_t* h_dist = anchor_hash_new();
    char node_name_dist[20];
    int counts_dist[NUM_NODES] = {0};

    // Add nodes
    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_name_dist, "node%d", i);
        anchor_hash_add(h_dist, node_name_dist);
    }

    // Hash keys and count distribution
    char key_dist[20];
    for (int i = 0; i < NUM_KEYS; i++) {
        sprintf(key_dist, "key%d", i);
        const char* node = anchor_hash_get(h_dist, key_dist);
        for (int j = 0; j < NUM_NODES; j++) {
            sprintf(node_name_dist, "node%d", j);
            if (strcmp(node, node_name_dist) == 0) {
                counts_dist[j]++;
                break;
            }
        }
    }

    // Output distribution results to CSV
    FILE* fp_dist = fopen("results.csv", "w");
    if (!fp_dist) {
        fprintf(stderr, "Failed to open results.csv\n");
        anchor_hash_free(h_dist);
        return 1;
    }

    fprintf(fp_dist, "Node,Keys,Algorithm\n");
    for (int i = 0; i < NUM_NODES; i++) {
        fprintf(fp_dist, "node%d,%d,anchor_c\n", i, counts_dist[i]);
    }
    fclose(fp_dist);
    anchor_hash_free(h_dist);
    printf("C key distribution experiment finished. Results saved to results.csv\n");

    // --- Remapping Experiment (New) ---
    anchor_hash_t* h_remap = anchor_hash_new();
    char initial_node[REMAP_KEYS][20];
    char remap_key[20];
    int remapped_on_remove = 0;
    int remapped_on_add = 0;

    // Add initial nodes
    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_name_dist, "node%d", i); // Reusing node_name_dist for convenience
        anchor_hash_add(h_remap, node_name_dist);
    }

    // Store initial mappings
    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* node = anchor_hash_get(h_remap, remap_key);
        if (node) {
            strcpy(initial_node[i], node);
        } else {
            strcpy(initial_node[i], "NULL"); // Handle case where node is not found
        }
    }

    // Remove one node (e.g., "node9")
    printf("Removing node9...\n");
    anchor_hash_remove(h_remap, "node9");

    // Count remappings after removal
    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* new_node = anchor_hash_get(h_remap, remap_key);
        if (new_node && strcmp(initial_node[i], new_node) != 0) {
            remapped_on_remove++;
        }
    }
    printf("Keys remapped after removal: %d\n", remapped_on_remove);

    // Add the node back
    printf("Adding node9 back...\n");
    anchor_hash_add(h_remap, "node9");

    // Count remappings after addition
    for (int i = 0; i < REMAP_KEYS; i++) {
        sprintf(remap_key, "remap_key%d", i);
        const char* new_node = anchor_hash_get(h_remap, remap_key);
        if (new_node && strcmp(initial_node[i], new_node) != 0) {
            remapped_on_add++;
        }
    }
    printf("Keys remapped after addition: %d\n", remapped_on_add);

    // Output remapping results to CSV
    FILE* fp_remap = fopen("remapping_results.csv", "w");
    if (!fp_remap) {
        fprintf(stderr, "Failed to open remapping_results.csv\n");
        anchor_hash_free(h_remap);
        return 1;
    }
    fprintf(fp_remap, "Operation,RemappedKeys,TotalKeys,Algorithm\n");
    fprintf(fp_remap, "Remove, %d, %d, anchor_c\n", remapped_on_remove, REMAP_KEYS);
    fprintf(fp_remap, "Add, %d, %d, anchor_c\n", remapped_on_add, REMAP_KEYS);
    fclose(fp_remap);
    anchor_hash_free(h_remap);
    printf("C remapping experiment finished. Results saved to remapping_results.csv\n");

    return 0;
}
