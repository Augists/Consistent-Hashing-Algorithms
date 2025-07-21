#include "dx_hash.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_NODES 1000
#define KEYS 1000000 // Increased keys for better statistical significance with more nodes

// For remapping curve experiments
#define REMOVAL_STEPS 11 // 0% to 50% removal in 5% increments
#define ADDITION_STEPS 11 // 0% to 50% addition in 5% increments (relative to removed nodes)
#define REMOVE_PERCENTAGE_FOR_ADDITION 0.20 // Remove 20% of nodes before adding back

// 峰均值比实验
double peak_to_average(dx_hash_t *ctx) {
    int *counts = calloc(ctx->N, sizeof(int));
    for (uint64_t k = 0; k < KEYS; ++k) {
        int b = dx_hash_map(ctx, k);
        counts[b]++;
    }
    int peak = 0, sum = 0;
    for (int i = 0; i < ctx->N; ++i) {
        if (ctx->A[i]) {
            if (counts[i] > peak) peak = counts[i];
            sum += counts[i];
        }
    }
    double avg = sum * 1.0 / ctx->a;
    free(counts);
    return peak / avg;
}

// 多步扩缩容重映射比例实验 (移除节点)，输出到 dx_remap_removal_curve.csv
void remap_removal_experiment() {
    FILE *f = fopen("results/dx_remap_removal_curve.csv", "w");
    if (!f) f = fopen("../results/dx_remap_removal_curve.csv", "w");
    if (!f) {
        printf("Cannot open results/dx_remap_removal_curve.csv for write!\n");
        return;
    }
    fprintf(f, "remove_ratio,remap_ratio\n");
    for (int step = 0; step < REMOVAL_STEPS; ++step) {
        double remove_ratio = (double)step / (REMOVAL_STEPS - 1) * 0.5; // 0% to 50%
        int remove_cnt = (int)(INITIAL_NODES * remove_ratio);
        if (remove_cnt > INITIAL_NODES) remove_cnt = INITIAL_NODES; // Cap at initial nodes

        dx_hash_t ctx;
        dx_hash_init(&ctx, INITIAL_NODES, INITIAL_NODES);
        int *before = malloc(KEYS * sizeof(int));
        for (uint64_t k = 0; k < KEYS; ++k)
            before[k] = dx_hash_map(&ctx, k);

        int removed_actual = 0;
        for (int i = 0; i < INITIAL_NODES && removed_actual < remove_cnt; ++i) {
            if (ctx.A[i]) { // Only remove if active
                dx_hash_remove(&ctx, i);
                removed_actual++;
            }
        }
        
        int changed = 0;
        for (uint64_t k = 0; k < KEYS; ++k) {
            int after = dx_hash_map(&ctx, k);
            if (after != before[k]) changed++;
        }
        free(before);
        double remap = changed * 1.0 / KEYS;
        fprintf(f, "%.2f,%.4f\n", (double)removed_actual / INITIAL_NODES, remap);
        dx_hash_free(&ctx);
    }
    fclose(f);
    printf("Remapping removal curve data written to results/dx_remap_removal_curve.csv\n");
}

// 多步扩缩容重映射比例实验 (添加节点)，输出到 dx_remap_addition_curve.csv
void remap_addition_experiment() {
    FILE *f = fopen("results/dx_remap_addition_curve.csv", "w");
    if (!f) f = fopen("../results/dx_remap_addition_curve.csv", "w");
    if (!f) {
        printf("Cannot open results/dx_remap_addition_curve.csv for write!\n");
        return;
    }
    fprintf(f, "add_ratio,remap_ratio\n");

    // Initial state: remove a fixed percentage of nodes
    int initial_removed_count = (int)(INITIAL_NODES * REMOVE_PERCENTAGE_FOR_ADDITION);
    if (initial_removed_count == 0 && INITIAL_NODES > 0) initial_removed_count = 1; // Ensure at least one node is removed if possible
    if (initial_removed_count > INITIAL_NODES) initial_removed_count = INITIAL_NODES;

    dx_hash_t base_ctx;
    dx_hash_init(&base_ctx, INITIAL_NODES, INITIAL_NODES);
    int *initial_mappings = malloc(KEYS * sizeof(int));
    for (uint64_t k = 0; k < KEYS; ++k) {
        initial_mappings[k] = dx_hash_map(&base_ctx, k);
    }

    // Perform initial removal
    int *removed_indices = malloc(initial_removed_count * sizeof(int));
    int removed_count_actual = 0;
    for (int i = 0; i < INITIAL_NODES && removed_count_actual < initial_removed_count; ++i) {
        if (base_ctx.A[i]) {
            dx_hash_remove(&base_ctx, i);
            removed_indices[removed_count_actual++] = i;
        }
    }

    // Store mappings after initial removal
    int *mappings_after_initial_removal = malloc(KEYS * sizeof(int));
    for (uint64_t k = 0; k < KEYS; ++k) {
        mappings_after_initial_removal[k] = dx_hash_map(&base_ctx, k);
    }

    // Now, add nodes back iteratively
    for (int step = 0; step < ADDITION_STEPS; ++step) {
        double add_ratio = (double)step / (ADDITION_STEPS - 1); // 0% to 100% of removed nodes added back
        int add_cnt = (int)(initial_removed_count * add_ratio);
        if (add_cnt > initial_removed_count) add_cnt = initial_removed_count;

        // Create a new context for each step to ensure clean state
        dx_hash_t current_ctx;
        dx_hash_init(&current_ctx, INITIAL_NODES, INITIAL_NODES - initial_removed_count); // Start with removed nodes
        
        // Re-remove nodes for current step
        for(int i = 0; i < removed_count_actual; ++i) {
            dx_hash_remove(&current_ctx, removed_indices[i]);
        }

        // Add back nodes for current step
        for (int i = 0; i < add_cnt; ++i) {
            dx_hash_add(&current_ctx, removed_indices[i]);
        }

        int changed = 0;
        for (uint64_t k = 0; k < KEYS; ++k) {
            int current_map = dx_hash_map(&current_ctx, k);
            if (current_map != mappings_after_initial_removal[k]) {
                changed++;
            }
        }
        fprintf(f, "%.2f,%.4f\n", (double)add_cnt / initial_removed_count, changed * 1.0 / KEYS);
        dx_hash_free(&current_ctx);
    }

    free(initial_mappings);
    free(mappings_after_initial_removal);
    free(removed_indices);
    dx_hash_free(&base_ctx);
    fclose(f);
    printf("Remapping addition curve data written to results/dx_remap_addition_curve.csv\n");
}


int main() {
    // 峰均值比实验
    dx_hash_t ctx;
    FILE *f = fopen("results/dx_experiment.csv", "w");
    if (!f) f = fopen("../results/dx_experiment.csv", "w");
    if (!f) {
        printf("Cannot open results/dx_experiment.csv for write!\n");
        return 1;
    }
    dx_hash_init(&ctx, INITIAL_NODES, INITIAL_NODES);
    double p2a = peak_to_average(&ctx);
    fprintf(f, "peak_to_average,%.4f\n", p2a);
    dx_hash_free(&ctx);
    fclose(f);
    printf("Peak/average data written to results/dx_experiment.csv\n");

    // 多步扩缩容重映射比例实验 (移除节点)
    remap_removal_experiment();

    // 多步扩缩容重映射比例实验 (添加节点)
    remap_addition_experiment();
    return 0;
}
