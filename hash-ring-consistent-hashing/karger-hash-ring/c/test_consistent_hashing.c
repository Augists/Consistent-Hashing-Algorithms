#include "consistent_hashing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_KEYS 10000
#define MAX_NODE_NAME_LEN 32

// Helper to print distribution
void print_distribution(const char* title, const char** nodes, int num_nodes, const char** keys, int num_keys, HashRing* hr) {
    printf("%s\n", title);
    
    // Simple hash map for counts
    typedef struct { char* node_name; int count; } NodeCount;
    NodeCount* counts = (NodeCount*)calloc(num_nodes, sizeof(NodeCount));
    for(int i=0; i<num_nodes; ++i) {
        counts[i].node_name = (char*)nodes[i];
    }

    for (int i = 0; i < num_keys; i++) {
        const char* assigned_node = get_node(hr, keys[i]);
        if (assigned_node) {
            for(int j=0; j<num_nodes; ++j) {
                if (strcmp(counts[j].node_name, assigned_node) == 0) {
                    counts[j].count++;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < num_nodes; i++) {
        if (counts[i].node_name) {
            printf("  %s: %d (%.2f%%)\n", counts[i].node_name, counts[i].count, (double)counts[i].count / num_keys * 100.0);
        }
    }
    free(counts);
}

int main() {
    srand(time(NULL));

    HashRing* hr = create_hash_ring(3); // 3 virtual nodes per real node
    if (!hr) {
        fprintf(stderr, "Failed to create hash ring.\n");
        return 1;
    }

    const char* initial_nodes[] = {"nodeA", "nodeB", "nodeC"};
    int num_initial_nodes = sizeof(initial_nodes) / sizeof(initial_nodes[0]);

    for (int i = 0; i < num_initial_nodes; i++) {
        add_node(hr, initial_nodes[i]);
    }

    char** keys = (char**)malloc(sizeof(char*) * NUM_KEYS);
    for (int i = 0; i < NUM_KEYS; i++) {
        keys[i] = (char*)malloc(sizeof(char) * MAX_NODE_NAME_LEN);
        snprintf(keys[i], MAX_NODE_NAME_LEN, "key%d", i);
    }

    print_distribution("Initial distribution:", initial_nodes, num_initial_nodes, (const char**)keys, NUM_KEYS, hr);

    // Add a new node and observe re-distribution
    printf("\nAdding nodeD...\n");
    add_node(hr, "nodeD");
    const char* nodes_after_add[] = {"nodeA", "nodeB", "nodeC", "nodeD"};
    print_distribution("Distribution after adding nodeD:", nodes_after_add, 4, (const char**)keys, NUM_KEYS, hr);

    // Remove a node and observe re-distribution
    printf("\nRemoving nodeB...\n");
    remove_node(hr, "nodeB");
    const char* nodes_after_remove[] = {"nodeA", "nodeC", "nodeD"};
    print_distribution("Distribution after removing nodeB:", nodes_after_remove, 3, (const char**)keys, NUM_KEYS, hr);

    // Test consistency for existing keys after node changes
    printf("\nTesting consistency for a few keys:\n");
    for (int i = 0; i < 5; i++) {
        int random_key_idx = rand() % NUM_KEYS;
        const char* key = keys[random_key_idx];
        const char* assigned_node = get_node(hr, key);
        printf("  Key %s maps to %s\n", key, assigned_node ? assigned_node : "(none)");
    }

    // Clean up
    for (int i = 0; i < NUM_KEYS; i++) {
        free(keys[i]);
    }
    free(keys);
    destroy_hash_ring(hr);

    return 0;
}