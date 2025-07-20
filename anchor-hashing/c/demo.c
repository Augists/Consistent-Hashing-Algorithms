#include <stdio.h>
#include "anchor_hash.h"

int main() {
    anchor_hash_t* h = anchor_hash_new();
    if (!h) {
        fprintf(stderr, "Failed to create anchor_hash_t\n");
        return 1;
    }

    // Add nodes
    anchor_hash_add(h, "node1");
    anchor_hash_add(h, "node2");
    anchor_hash_add(h, "node3");
    anchor_hash_add(h, "node4");
    anchor_hash_add(h, "node5");

    // Get a key
    const char* key = "my-test-key";
    const char* node = anchor_hash_get(h, key);

    if (node) {
        printf("Key '%s' is mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    // Remove a node and test again
    anchor_hash_remove(h, "node3");
    printf("\nAfter removing 'node3':\n");
    node = anchor_hash_get(h, key);

    if (node) {
        printf("Key '%s' is now mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    anchor_hash_free(h);
    return 0;
}
