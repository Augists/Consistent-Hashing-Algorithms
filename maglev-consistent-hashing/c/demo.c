#include <stdio.h>
#include <stdlib.h>
#include "maglev.h"

int main() {
    // 1. Define backend nodes and lookup table size.
    const char* nodes[] = {
        "backend-alpha",
        "backend-beta",
        "backend-gamma",
        "backend-delta",
    };
    size_t N = sizeof(nodes) / sizeof(nodes[0]);
    size_t M = 65537; // A large prime number

    printf("Initializing Maglev with %zu nodes and table size M=%zu\n", N, M);
    printf("Nodes: [");
    for (size_t i = 0; i < N; i++) {
        printf("\"%s\"%s", nodes[i], i == N - 1 ? "" : ", ");
    }
    printf("]\n--------------------------------------------------\n");

    // 2. Create a new Maglev instance.
    maglev_t* m = maglev_new(nodes, N, M);
    if (!m) {
        fprintf(stderr, "Failed to create Maglev instance\n");
        return 1;
    }

    // 3. Simulate hashing some keys.
    printf("Hashing some sample keys:\n");
    const char* keys_to_test[] = {
        "customer:12345",
        "session:abcdef",
        "request:xyz789",
        "user_profile:jane.doe",
        "customer:12345", // Test consistency
    };
    size_t num_keys = sizeof(keys_to_test) / sizeof(keys_to_test[0]);

    for (size_t i = 0; i < num_keys; i++) {
        const char* backend = maglev_get(m, keys_to_test[i]);
        printf("  - Key '%s' -> %s\n", keys_to_test[i], backend);
    }
    printf("--------------------------------------------------\n");

    // 4. Show the distribution of the lookup table.
    printf("Lookup table distribution:\n");
    int* counts = calloc(N, sizeof(int));
    if (!counts) {
        maglev_free(m);
        return 1;
    }

    for (size_t i = 0; i < M; i++) {
        counts[m->lookup[i]]++;
    }

    for (size_t i = 0; i < N; i++) {
        double percentage = 100.0 * counts[i] / M;
        // Note: m->nodes is sorted, so we need to find the original node name
        // for printing, or just print the sorted ones. We'll print the sorted ones.
        printf("  - %s: %d entries (%.2f%%)\n", m->nodes[i], counts[i], percentage);
    }
    printf("--------------------------------------------------\n");

    // 5. Clean up.
    free(counts);
    maglev_free(m);

    return 0;
}