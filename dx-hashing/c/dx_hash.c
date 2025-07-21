#include "dx_hash.h"
#include <stdlib.h>
#include <string.h>

// Simple hash function (can be replaced with a better one)
static uint64_t simple_hash(uint64_t key, int salt) {
    key ^= (uint64_t)salt * 0x9e3779b97f4a7c15ULL;
    key ^= (key >> 33);
    key *= 0xff51afd7ed558ccdULL;
    key ^= (key >> 33);
    key *= 0xc4ceb9fe1a85ec53ULL;
    key ^= (key >> 33);
    return key;
}

int dx_hash_init(dx_hash_t *ctx, int N, int a) {
    if (a > N || a <= 0 || N <= 0) return -1;
    ctx->N = N;
    ctx->a = a;
    ctx->A = malloc(N * sizeof(int));
    if (!ctx->A) return -1;
    for (int i = 0; i < N; ++i) {
        ctx->A[i] = (i < a) ? 1 : 0;
    }
    return 0;
}

void dx_hash_free(dx_hash_t *ctx) {
    free(ctx->A);
}

// DxHash: double hashing (jump style)
int dx_hash_map(dx_hash_t *ctx, uint64_t key) {
    int N = ctx->N;
    int h1 = simple_hash(key, 0) % N;
    int h2 = 1 + (simple_hash(key, 1) % (N - 1));
    int b = h1;
    for (int i = 0; i < N; ++i) {
        if (ctx->A[b]) return b;
        b = (b + h2) % N;
    }
    return -1;
}

int dx_hash_remove(dx_hash_t *ctx, int b) {
    if (!ctx->A[b] || ctx->a <= 1) return -1;
    ctx->A[b] = 0;
    ctx->a--;
    return 0;
}

int dx_hash_add(dx_hash_t *ctx, int b) {
    if (ctx->A[b] || ctx->a >= ctx->N) return -1;
    ctx->A[b] = 1;
    ctx->a++;
    return 0;
}
