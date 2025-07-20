#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multi_probe_hashing.h"

#define NUM_NODES 10
#define NUM_KEYS 100000
#define REPLICAS 20
#define PROBES 5

int main() {
    MultiProbeHashRing* hr = create_multi_probe_hash_ring(REPLICAS, PROBES);
    char node_names[NUM_NODES][20];
    int counts[NUM_NODES] = {0};

    // Add nodes
    for (int i = 0; i < NUM_NODES; i++) {
        sprintf(node_names[i], "node%d", i);
        add_multi_probe_node(hr, node_names[i]);
    }

    // Hash keys and count distribution
    char key[20];
    for (int i = 0; i < NUM_KEYS; i++) {
        sprintf(key, "key%d", i);
        const char* node = get_multi_probe_node(hr, key);
        for (int j = 0; j < NUM_NODES; j++) {
            if (strcmp(node, node_names[j]) == 0) {
                counts[j]++;
                break;
            }
        }
    }

    // Output results to CSV
    FILE* fp = fopen("results.csv", "w");
    if (!fp) {
        fprintf(stderr, "Failed to open results.csv\n");
        destroy_multi_probe_hash_ring(hr);
        return 1;
    }

    fprintf(fp, "Node,Keys,Algorithm\n");
    for (int i = 0; i < NUM_NODES; i++) {
        fprintf(fp, "node%d,%d,multiprobe_c\n", i, counts[i]);
    }

    fclose(fp);
    destroy_multi_probe_hash_ring(hr);
    printf("C experiment finished. Results saved to results.csv\n");

    return 0;
}
