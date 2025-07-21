
#include "anchor_hash.h"
#include <stdlib.h>
#include <string.h>

// Murmur-inspired fast hash
static uint64_t simple_hash(uint64_t key, int salt) {
    key ^= (uint64_t)salt * 0x9e3779b97f4a7c15ULL;
    key ^= (key >> 33);
    key *= 0xff51afd7ed558ccdULL;
    key ^= (key >> 33);
    key *= 0xc4ceb9fe1a85ec53ULL;
    key ^= (key >> 33);
    return key;
}

// AnchorHash 正统实现
int anchor_hash_init(anchor_hash_t *ctx, int N, int a) {
    if (a > N || a <= 0 || N <= 0) return -1;
    ctx->N = N;
    ctx->a = a;
    ctx->A = malloc(N * sizeof(int));
    ctx->W = malloc(N * sizeof(int));
    ctx->L = malloc(N * sizeof(int));
    ctx->R = malloc(N * sizeof(int));
    if (!ctx->A || !ctx->W || !ctx->L || !ctx->R) return -1;
    for (int i = 0; i < N; ++i) {
        ctx->A[i] = (i < a) ? 1 : 0;
        ctx->W[i] = i;
        ctx->L[i] = i;
        ctx->R[i] = -1; // 记录恢复链
    }
    return 0;
}

void anchor_hash_free(anchor_hash_t *ctx) {
    free(ctx->A); free(ctx->W); free(ctx->L); free(ctx->R);
}

// AnchorHash 映射，按论文推荐的“工作集+历史链”
int anchor_hash_map(anchor_hash_t *ctx, uint64_t key) {
    int b = simple_hash(key, 0) % ctx->N;
    while (!ctx->A[b]) {
        b = ctx->R[b];
        if (b < 0) return -1; // 不应发生
    }
    return b;
}

// 移除 bucket b，记录恢复链
int anchor_hash_remove(anchor_hash_t *ctx, int b) {
    if (!ctx->A[b] || ctx->a <= 1) return -1;
    int pos = ctx->L[b];
    int last = ctx->a - 1;
    int last_b = ctx->W[last];
    ctx->W[pos] = ctx->W[last];
    ctx->L[last_b] = pos;
    ctx->a--;
    ctx->A[b] = 0;
    ctx->R[b] = last_b; // 记录被移除时的“父”
    return 0;
}

// 恢复 bucket b，恢复链回溯
int anchor_hash_add(anchor_hash_t *ctx, int b) {
    if (ctx->A[b] || ctx->a >= ctx->N) return -1;
    ctx->A[b] = 1;
    ctx->W[ctx->a] = b;
    ctx->L[b] = ctx->a;
    ctx->a++;
    // 恢复链断开
    ctx->R[b] = -1;
    return 0;
}
