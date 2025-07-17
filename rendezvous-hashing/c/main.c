#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Simple FNV-1a hash for demonstration
// In a real application, use a more robust hashing library.
uint64_t fnv1a_64(const char *s) {
    uint64_t hash = 0xcbf29ce484222325ULL; // FNV offset basis
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint64_t)s[i];
        hash *= 0x100000001b3ULL; // FNV prime
    }
    return hash;
}

// RendezvousHash calculates the highest random weight for a given key and a list of nodes.
// It returns the index of the chosen node.
int rendezvous_hash(const char* key, char** nodes, int num_nodes) {
    if (num_nodes == 0) {
        return -1; // No nodes available
    }

    double max_weight = -1.0;
    int chosen_idx = -1;

    for (int i = 0; i < num_nodes; i++) {
        // Combine key and node name to create a unique string for hashing
        char combined[256];
        snprintf(combined, sizeof(combined), "%s-%s", key, nodes[i]);
        
        uint64_t hash_value = fnv1a_64(combined);

        // For true HRW, a more robust pseudo-random number generator seeded by the combined hash
        // would be used, often involving floating-point numbers for better distribution.
        // For demonstration, a direct use of hash_value is sufficient to show the principle.
        double weight = (double)hash_value; // Using hash value directly as weight for simplicity

        if (weight > max_weight) {
            max_weight = weight;
            chosen_idx = i;
        }
    }
    return chosen_idx;
}

// Helper to print distribution
void print_distribution(const char* title, char** nodes, int num_nodes, char** keys, int num_keys) {
    printf("%s\n", title);
    
    // Simple hash map for counts
    typedef struct { char* node_name; int count; } NodeCount;
    NodeCount* counts = (NodeCount*)calloc(num_nodes, sizeof(NodeCount));
    for(int i=0; i<num_nodes; ++i) {
        counts[i].node_name = nodes[i];
    }

    for (int i = 0; i < num_keys; i++) {
        int assigned_idx = rendezvous_hash(keys[i], nodes, num_nodes);
        if (assigned_idx != -1) {
            for(int j=0; j<num_nodes; ++j) {
                if (strcmp(counts[j].node_name, nodes[assigned_idx]) == 0) {
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
    printf("Rendezvous Hashing Demonstration\n");

    char* initial_nodes[] = {"serverA", "serverB", "serverC"};
    int num_initial_nodes = sizeof(initial_nodes) / sizeof(initial_nodes[0]);
    printf("Initial Nodes: ");
    for(int i=0; i<num_initial_nodes; ++i) printf("%s ", initial_nodes[i]);
    printf("\n");

    int num_keys = 10000;
    char** keys = (char**)malloc(sizeof(char*) * num_keys);
    for (int i = 0; i < num_keys; i++) {
        keys[i] = (char*)malloc(sizeof(char) * 32);
        snprintf(keys[i], 32, "data_key_%d", i);
    }

    print_distribution("\nInitial Distribution:", initial_nodes, num_initial_nodes, keys, num_keys);

    // Add a new node
    printf("\nAdding serverD...\n");
    char* nodes_after_add[] = {"serverA", "serverB", "serverC", "serverD"};
    int num_nodes_after_add = sizeof(nodes_after_add) / sizeof(nodes_after_add[0]);
    print_distribution("Distribution after adding serverD:", nodes_after_add, num_nodes_after_add, keys, num_keys);

    // Observe reassignments
    printf("\nObserving reassignments for a few keys:\n");
    char* keys_to_observe[] = {"data_key_100", "data_key_500", "data_key_9999"};
    for (int i = 0; i < sizeof(keys_to_observe) / sizeof(keys_to_observe[0]); i++) {
        const char* key = keys_to_observe[i];
        int initial_node_idx = rendezvous_hash(key, initial_nodes, num_initial_nodes);
        int new_node_idx = rendezvous_hash(key, nodes_after_add, num_nodes_after_add);
        printf("  Key %s: Initial -> %s, After adding serverD -> %s\n", 
               key, 
               initial_node_idx != -1 ? initial_nodes[initial_node_idx] : "(none)", 
               new_node_idx != -1 ? nodes_after_add[new_node_idx] : "(none)");
    }

    // Remove a node
    printf("\nRemoving serverB...\n");
    char* final_nodes[] = {"serverA", "serverC", "serverD"};
    int num_final_nodes = sizeof(final_nodes) / sizeof(final_nodes[0]);
    print_distribution("Distribution after removing serverB:", final_nodes, num_final_nodes, keys, num_keys);

    // Clean up
    for (int i = 0; i < num_keys; i++) {
        free(keys[i]);
    }
    free(keys);

    return 0;
}