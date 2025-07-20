#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maglev.h"

#define N_NODES 10 // Number of nodes for distribution experiment
#define M_LOOKUP_TABLE_SIZE 65537 // Size of the lookup table (prime number)
#define NUM_KEYS_DISTRIBUTION 1000000 // Number of keys for distribution experiment

// For k-failure experiment
#define N_K_FAILURE 100 // Total number of nodes in the healthy state for k-failure
#define M_K_FAILURE 65537 // Size of the Maglev lookup table for k-failure

int main() {
    // --- Key Distribution Experiment ---
    // Generate node names for distribution experiment
    const char** dist_nodes = malloc(sizeof(char*) * N_NODES);
    if (!dist_nodes) return 1;
    for (int i = 0; i < N_NODES; i++) {
        char buf[20];
        sprintf(buf, "node%d", i);
        dist_nodes[i] = strdup(buf);
        if (!dist_nodes[i]) return 1;
    }

    maglev_t* dist_maglev = maglev_new(dist_nodes, N_NODES, M_LOOKUP_TABLE_SIZE);
    if (!dist_maglev) {
        fprintf(stderr, "Failed to create Maglev table for distribution experiment\n");
        return 1;
    }

    int* counts_dist = calloc(N_NODES, sizeof(int));
    if (!counts_dist) return 1;

    char key_buf[30];
    for (uint64_t i = 0; i < NUM_KEYS_DISTRIBUTION; i++) {
        sprintf(key_buf, "key-%llu", i);
        const char* assigned_node = maglev_get(dist_maglev, key_buf);
        for (int j = 0; j < N_NODES; j++) {
            if (strcmp(assigned_node, dist_nodes[j]) == 0) {
                counts_dist[j]++;
                break;
            }
        }
    }

    printf("MAGLEV_DIST_START\n");
    printf("Node,Keys,Algorithm\n");
    for (int i = 0; i < N_NODES; i++) {
        printf("node%d,%d,maglev_c\n", i, counts_dist[i]);
    }
    printf("MAGLEV_DIST_END\n");

    free(counts_dist);
    maglev_free(dist_maglev);
    for (int i = 0; i < N_NODES; i++) {
        free((void*)dist_nodes[i]);
    }
    free(dist_nodes);

    // --- k-Failure Disruption Experiment (Existing, modified for stdout) ---
    // Generate node names for k-failure experiment
    const char** k_failure_nodes = malloc(sizeof(char*) * N_K_FAILURE);
    if (!k_failure_nodes) return 1;
    for (int i = 0; i < N_K_FAILURE; i++) {
        char buf[20];
        sprintf(buf, "backend-%03d", i);
        k_failure_nodes[i] = strdup(buf);
        if (!k_failure_nodes[i]) return 1;
    }

    maglev_t* healthy_maglev = maglev_new(k_failure_nodes, N_K_FAILURE, M_K_FAILURE);
    if (!healthy_maglev) {
        fprintf(stderr, "Failed to create healthy Maglev table for k-failure experiment\n");
        return 1;
    }

    printf("MAGLEV_REMAP_START\n");
    printf("k,disruption_percent,Algorithm\n"); // Added Algorithm column

    // Simulate removing k nodes, from 1 to N_K_FAILURE-1
    for (int k = 1; k < N_K_FAILURE; k++) {
        const char** unhealthy_nodes = &k_failure_nodes[k];
        size_t unhealthy_n = N_K_FAILURE - k;

        maglev_t* unhealthy_maglev = maglev_new(unhealthy_nodes, unhealthy_n, M_K_FAILURE);
        if (!unhealthy_maglev) {
            fprintf(stderr, "Failed to create unhealthy Maglev table for k=%d\n", k);
            return 1;
        }

        int disruptions = 0;
        int valid_entries = 0;

        for (size_t i = 0; i < M_K_FAILURE; i++) {
            int healthy_node_index = healthy_maglev->lookup[i];
            const char* healthy_node_name = healthy_maglev->nodes[healthy_node_index];

            int is_failed = 0;
            for (int j = 0; j < k; j++) {
                if (strcmp(healthy_node_name, k_failure_nodes[j]) == 0) {
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

        printf("%d,%.4f,maglev_c\n", k, disruption_percent); // Added Algorithm column

        maglev_free(unhealthy_maglev);
    }
    printf("MAGLEV_REMAP_END\n");

    // Cleanup for k-failure experiment
    maglev_free(healthy_maglev);
    for (int i = 0; i < N_K_FAILURE; i++) {
        free((void*)k_failure_nodes[i]);
    }
    free(k_failure_nodes);

    return 0;
}