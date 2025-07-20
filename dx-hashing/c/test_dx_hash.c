#include "dx_hash.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_add_and_get() {
    printf("Running test_add_and_get...\n");
    dx_hash_t* h = dx_hash_new(10);
    assert(h != NULL);

    dx_hash_add(h, "node1");
    dx_hash_add(h, "node2");
    dx_hash_add(h, "node3");

    const char* node = dx_hash_get(h, "my-key");
    assert(node != NULL);
    printf("Key 'my-key' mapped to node: %s\n", node);

    dx_hash_free(h);
    printf("test_add_and_get passed.\n");
}

void test_remove() {
    printf("Running test_remove...\n");
    dx_hash_t* h = dx_hash_new(10);
    assert(h != NULL);

    dx_hash_add(h, "node1");
    dx_hash_add(h, "node2");
    dx_hash_add(h, "node3");

    dx_hash_remove(h, "node2");
    const char* node = dx_hash_get(h, "another-key");
    assert(node != NULL);
    assert(strcmp(node, "node2") != 0);
    printf("Key 'another-key' mapped to node: %s\n", node);

    dx_hash_free(h);
    printf("test_remove passed.\n");
}

void test_full() {
    printf("Running test_full...\n");
    dx_hash_t* h = dx_hash_new(2);
    assert(h != NULL);

    dx_hash_add(h, "node1");
    dx_hash_add(h, "node2");
    int result = dx_hash_add(h, "node3");
    assert(result == -1);

    dx_hash_free(h);
    printf("test_full passed.\n");
}

int main() {
    test_add_and_get();
    test_remove();
    test_full();
    return 0;
}
