#include <stdio.h>
#include "multi_probe_hashing.h"

#define REPLICAS 10
#define PROBES 5

int main() {
    MultiProbeHashRing* hr = create_multi_probe_hash_ring(REPLICAS, PROBES);
    if (!hr) {
        fprintf(stderr, "Failed to create hash ring\n");
        return 1;
    }

    // Add nodes
    add_multi_probe_node(hr, "node1");
    add_multi_probe_node(hr, "node2");
    add_multi_probe_node(hr, "node3");

    // Get a key
    const char* key = "my-special-key";
    const char* node = get_multi_probe_node(hr, key);

    if (node) {
        printf("Key '%s' is mapped to node '%s'\n", key, node);
    } else {
        printf("Could not find a node for key '%s'\n", key);
    }

    destroy_multi_probe_hash_ring(hr);
    return 0;
}
