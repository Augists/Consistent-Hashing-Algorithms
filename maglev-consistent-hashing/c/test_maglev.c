#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "maglev.h"

// Helper to print test names
void test_name(const char* name) {
    printf("--- Running test: %s ---\n", name);
}

void test_creation() {
    test_name("Creation and Destruction");
    const char* nodes[] = {"backend-0", "backend-1", "backend-2"};
    size_t N = 3;
    size_t M = 101;

    maglev_t* m = maglev_new(nodes, N, M);
    assert(m != NULL);
    assert(m->N == N);
    assert(m->M == M);
    assert(m->lookup != NULL);
    assert(m->nodes != NULL);
    printf("Creation successful.\n");

    maglev_free(m);
    printf("Destruction successful.\n");

    // Test invalid inputs
    m = maglev_new(NULL, 0, M);
    assert(m == NULL);
    m = maglev_new(nodes, N, 0);
    assert(m == NULL);
    printf("Invalid input handling successful.\n");
}

void test_consistency() {
    test_name("Consistency");
    const char* nodes[] = {"backend-0", "backend-1", "backend-2"};
    size_t N = 3;
    size_t M = 101;
    maglev_t* m = maglev_new(nodes, N, M);

    const char* key = "my-consistent-key";
    const char* node1 = maglev_get(m, key);
    const char* node2 = maglev_get(m, key);

    assert(strcmp(node1, node2) == 0);
    printf("Get is consistent: '%s' -> '%s'\n", key, node1);

    maglev_free(m);
}

void test_rebalancing() {
    test_name("Rebalancing");
    size_t N = 10;
    size_t M = 65537;
    const char** nodesN = malloc(sizeof(char*) * N);
    assert(nodesN);
    for (size_t i = 0; i < N; i++) {
        char buf[20];
        sprintf(buf, "backend-%02zu", i);
        nodesN[i] = strdup(buf);
        assert(nodesN[i]);
    }

    maglev_t* maglevN = maglev_new(nodesN, N, M);
    assert(maglevN);

    // N-1 nodes
    maglev_t* maglevN1 = maglev_new(nodesN, N - 1, M);
    assert(maglevN1);

    int num_keys = 100000;
    int changes = 0;
    const char* removed_node = nodesN[N - 1];

    for (int i = 0; i < num_keys; i++) {
        char key[20];
        sprintf(key, "key-%d", i);
        const char* nodeN = maglev_get(maglevN, key);
        const char* nodeN1 = maglev_get(maglevN1, key);

        if (strcmp(nodeN, removed_node) != 0 && strcmp(nodeN, nodeN1) != 0) {
            changes++;
        }
    }

    double change_rate = (double)changes / num_keys;
    printf("Found %d unexpected changes out of %d keys (%.4f%%)\n", changes, num_keys, change_rate * 100);
    assert(change_rate < 0.002); // Same threshold as Go test

    for (size_t i = 0; i < N; i++) {
        free((void*)nodesN[i]);
    }
    free(nodesN);
    maglev_free(maglevN);
    maglev_free(maglevN1);
}


int main() {
    test_creation();
    printf("\n");
    test_consistency();
    printf("\n");
    test_rebalancing();
    printf("\n");

    printf("All C tests passed!\n");
    return 0;
}
