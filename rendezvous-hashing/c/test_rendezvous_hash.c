#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "rendezvous_hash.h"

void test_fnv1a_64() {
    printf("Running test_fnv1a_64...\n");
    assert(fnv1a_64("test") == 0xaf63dc4c860698f5ULL);
    assert(fnv1a_64("") == 0xcbf29ce484222325ULL);
    printf("test_fnv1a_64 passed.\n");
}

void test_rendezvous_hash_basic() {
    printf("Running test_rendezvous_hash_basic...\n");
    char* nodes[] = {"nodeA", "nodeB", "nodeC"};
    int num_nodes = 3;
    const char* key = "mykey";

    int chosen_idx = rendezvous_hash(key, nodes, num_nodes);
    assert(chosen_idx >= 0 && chosen_idx < num_nodes);
    printf("  Key '%s' mapped to '%s'.\n", key, nodes[chosen_idx]);

    // Test with a different key
    const char* key2 = "anotherkey";
    int chosen_idx2 = rendezvous_hash(key2, nodes, num_nodes);
    assert(chosen_idx2 >= 0 && chosen_idx2 < num_nodes);
    printf("  Key '%s' mapped to '%s'.\n", key2, nodes[chosen_idx2]);

    printf("test_rendezvous_hash_basic passed.\n");
}

void test_rendezvous_hash_empty_nodes() {
    printf("Running test_rendezvous_hash_empty_nodes...\n");
    char* nodes[] = {};
    int num_nodes = 0;
    const char* key = "mykey";

    int chosen_idx = rendezvous_hash(key, nodes, num_nodes);
    assert(chosen_idx == -1);
    printf("  Correctly returned -1 for empty nodes.\n");
    printf("test_rendezvous_hash_empty_nodes passed.\n");
}

int main() {
    test_fnv1a_64();
    test_rendezvous_hash_basic();
    test_rendezvous_hash_empty_nodes();
    printf("\nAll C tests passed for Rendezvous Hashing!\n");
    return 0;
}
