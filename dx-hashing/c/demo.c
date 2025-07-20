#include <stdio.h>
#include "dx_hash.h"

int main() {
    dx_hash_t* h = dx_hash_new(10);
    if (!h) {
        fprintf(stderr, "Failed to create dx_hash_t\n");
        return 1;
    }

    // Add nodes
    dx_hash_add(h, "node1");
    dx_hash_add(h, "node2");
    dx_hash_add(h, "node3");
    dx_hash_add(h, "node4");
    dx_hash_add(h, "node5");

    // Get a key
    const char* key = "my-test-key";
    const char* node = dx_hash_get(h, key);

    if (node) {
        printf("Key '%s' is mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    // Remove a node and test again
    dx_hash_remove(h, "node3");
    printf("\nAfter removing 'node3':\n");
    node = dx_hash_get(h, key);

    if (node) {
        printf("Key '%s' is now mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    dx_hash_free(h);
    return 0;
}
