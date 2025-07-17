#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maglev.h"

#define N 100
#define M 65537

int main() {
    // Generate node names
    const char** nodes = malloc(sizeof(char*) * N);
    if (!nodes) return 1;
    for (int i = 0; i < N; i++) {
        char buf[20];
        sprintf(buf, "backend-%03d", i);
        nodes[i] = strdup(buf);
        if (!nodes[i]) return 1; // Handle allocation failure
    }

    // Create the initial, healthy Maglev table
    maglev_t* healthy_maglev = maglev_new(nodes, N, M);
    if (!healthy_maglev) {
        fprintf(stderr, "Failed to create healthy Maglev table\n");
        return 1;
    }

    printf("k,disruption_percent\n");

    // Simulate removing k nodes, from 1 to N-1
    for (int k = 1; k < N; k++) {
        // The first k nodes are considered "failed"
        // We can just pass the sub-array of nodes to maglev_new
        const char** unhealthy_nodes = &nodes[k];
        int unhealthy_n = N - k;

        maglev_t* unhealthy_maglev = maglev_new(unhealthy_nodes, unhealthy_n, M);
        if (!unhealthy_maglev) {
            fprintf(stderr, "Failed to create unhealthy Maglev table for k=%d\n", k);
            return 1;
        }

        // Compare the lookup tables
        int disruptions = 0;
        int valid_entries = 0;

        for (int i = 0; i < M; i++) {
            int healthy_node_index = healthy_maglev->lookup[i];
            const char* healthy_node_name = healthy_maglev->nodes[healthy_node_index];

            // Check if the original node is in the failed set
            int is_failed = 0;
            for (int j = 0; j < k; j++) {
                if (strcmp(healthy_node_name, nodes[j]) == 0) {
                    is_failed = 1;
                    break;
                }
            }

            if (is_failed) {
                continue;
            }
            valid_entries++;

            int unhealthy_node_index = unhealthy_maglev->lookup[i];
            const char* unhealthy_node_name = unhealthy_maglev->nodes[unhealthy_node_index];

            if (strcmp(healthy_node_name, unhealthy_node_name) != 0) {
                disruptions++;
            }
        }

        double disruption_percent = 0.0;
        if (valid_entries > 0) {
            disruption_percent = (double)disruptions / valid_entries * 100.0;
        }

        printf("%d,%.4f\n", k, disruption_percent);

        maglev_free(unhealthy_maglev);
    }

    // Cleanup
    maglev_free(healthy_maglev);
    for (int i = 0; i < N; i++) {
        free((void*)nodes[i]);
    }
    free(nodes);

    return 0;
}