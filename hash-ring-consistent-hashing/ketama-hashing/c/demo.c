#include <stdio.h>
#include "ketama.h"

int main() {
    KetamaRing* kr = create_ketama_ring(160); // 160 virtual nodes per real node
    if (!kr) {
        fprintf(stderr, "Failed to create Ketama ring\n");
        return 1;
    }

    // Add nodes
    add_ketama_node(kr, "node1");
    add_ketama_node(kr, "node2");
    add_ketama_node(kr, "node3");
    add_ketama_node(kr, "node4");
    add_ketama_node(kr, "node5");

    // Get a key
    const char* key = "my-test-key";
    const char* node = get_ketama_node(kr, key);

    if (node) {
        printf("Key '%s' is mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    // Remove a node and test again
    remove_ketama_node(kr, "node3");
    printf("\nAfter removing 'node3':\n");
    node = get_ketama_node(kr, key);

    if (node) {
        printf("Key '%s' is now mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    destroy_ketama_ring(kr);
    return 0;
}
