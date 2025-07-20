#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rendezvous_hash.h"

#define NUM_NODES 10
#define NUM_KEYS 1000000

int main() {
    char* nodes[NUM_NODES];
    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i] = (char*)malloc(sizeof(char) * 20);
        sprintf(nodes[i], "node%d", i);
    }

    int counts[NUM_NODES] = {0};
    char key[20];

    for (int i = 0; i < NUM_KEYS; i++) {
        sprintf(key, "key%d", i);
        int assigned_idx = rendezvous_hash(key, nodes, NUM_NODES);
        if (assigned_idx != -1) {
            counts[assigned_idx]++;
        }
    }

    // Output results to CSV
    FILE* fp = fopen("results.csv", "w");
    if (!fp) {
        fprintf(stderr, "Failed to open results.csv\n");
        for (int i = 0; i < NUM_NODES; i++) free(nodes[i]);
        return 1;
    }

    fprintf(fp, "Node,Keys,Algorithm\n");
    for (int i = 0; i < NUM_NODES; i++) {
        fprintf(fp, "%s,%d,rendezvous_c\n", nodes[i], counts[i]);
    }

    fclose(fp);
    printf("C experiment finished. Results saved to results.csv\n");

    for (int i = 0; i < NUM_NODES; i++) {
        free(nodes[i]);
    }

    return 0;
}
