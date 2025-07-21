#ifndef DX_HASH_H
#define DX_HASH_H
#include <stdint.h>
#include <stddef.h>

// DxHash context
typedef struct {
    int *A;        // Active buckets
    int N;         // Total buckets
    int a;         // Number of active buckets
} dx_hash_t;

// Initialize DxHash context with N buckets and a active buckets
// Returns 0 on success, -1 on failure
int dx_hash_init(dx_hash_t *ctx, int N, int a);

// Free DxHash context
void dx_hash_free(dx_hash_t *ctx);

// Map a key to a bucket
int dx_hash_map(dx_hash_t *ctx, uint64_t key);

// Remove a bucket (simulate node removal)
int dx_hash_remove(dx_hash_t *ctx, int b);

// Add a bucket (simulate node addition)
int dx_hash_add(dx_hash_t *ctx, int b);

#endif // DX_HASH_H
