#include "rendezvous_hash.h"
#include <string.h>
#include <stdio.h> // For snprintf

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
