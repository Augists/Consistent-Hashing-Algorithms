#include <stdio.h>
#include "consistent_hashing.h"

int main() {
    HashRing* hr = create_hash_ring(100); // 100 virtual nodes per real node
    if (!hr) {
        fprintf(stderr, "Failed to create hash ring\n");
        return 1;
    }

    // Add nodes
    add_node(hr, "node1");
    add_node(hr, "node2");
    add_node(hr, "node3");
    add_node(hr, "node4");
    add_node(hr, "node5");

    // Get a key
    const char* key = "my-test-key";
    const char* node = get_node(hr, key);

    if (node) {
        printf("Key '%s' is mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    // Remove a node and test again
    remove_node(hr, "node3");
    printf("\nAfter removing 'node3':\n");
    node = get_node(hr, key);

    if (node) {
        printf("Key '%s' is now mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    destroy_hash_ring(hr);
    return 0;
}
