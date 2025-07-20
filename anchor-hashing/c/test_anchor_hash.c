#include "anchor_hash.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_add_and_get() {
    printf("Running test_add_and_get...\n");
    anchor_hash_t* h = anchor_hash_new();
    assert(h != NULL);

    anchor_hash_add(h, "node1");
    anchor_hash_add(h, "node2");
    anchor_hash_add(h, "node3");

    const char* node = anchor_hash_get(h, "my-key");
    assert(node != NULL);
    printf("Key 'my-key' mapped to node: %s\n", node);

    anchor_hash_free(h);
    printf("test_add_and_get passed.\n");
}

void test_remove() {
    printf("Running test_remove...\n");
    anchor_hash_t* h = anchor_hash_new();
    assert(h != NULL);

    anchor_hash_add(h, "node1");
    anchor_hash_add(h, "node2");
    anchor_hash_add(h, "node3");

    anchor_hash_remove(h, "node2");
    const char* node = anchor_hash_get(h, "another-key");
    assert(node != NULL);
    assert(strcmp(node, "node2") != 0);
    printf("Key 'another-key' mapped to node: %s\n", node);


    anchor_hash_free(h);
    printf("test_remove passed.\n");
}

int main() {
    test_add_and_get();
    test_remove();
    return 0;
}
