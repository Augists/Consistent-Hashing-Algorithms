#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rendezvous_hash.h"

int main() {
    printf("Rendezvous Hashing Demonstration\n");

    char* initial_nodes[] = {"serverA", "serverB", "serverC"};
    int num_initial_nodes = sizeof(initial_nodes) / sizeof(initial_nodes[0]);
    printf("Initial Nodes: ");
    for(int i=0; i<num_initial_nodes; ++i) printf("%s ", initial_nodes[i]);
    printf("\n");

    const char* key = "my-test-key";
    int assigned_idx = rendezvous_hash(key, initial_nodes, num_initial_nodes);

    if (assigned_idx != -1) {
        printf("Key '%s' is mapped to node '%s'\n", key, initial_nodes[assigned_idx]);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    // Demonstrate adding a node
    printf("\nAdding serverD...\n");
    char* nodes_after_add[] = {"serverA", "serverB", "serverC", "serverD"};
    int num_nodes_after_add = sizeof(nodes_after_add) / sizeof(nodes_after_add[0]);
    
    assigned_idx = rendezvous_hash(key, nodes_after_add, num_nodes_after_add);
    if (assigned_idx != -1) {
        printf("Key '%s' is now mapped to node '%s'\n", key, nodes_after_add[assigned_idx]);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    // Demonstrate removing a node
    printf("\nRemoving serverB...\n");
    char* final_nodes[] = {"serverA", "serverC", "serverD"};
    int num_final_nodes = sizeof(final_nodes) / sizeof(final_nodes[0]);

    assigned_idx = rendezvous_hash(key, final_nodes, num_final_nodes);
    if (assigned_idx != -1) {
        printf("Key '%s' is now mapped to node '%s'\n", key, final_nodes[assigned_idx]);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    return 0;
}