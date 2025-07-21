#ifndef ANCHOR_HASH_H
#define ANCHOR_HASH_H
#include <stdint.h>
#include <stddef.h>

// AnchorHash context
typedef struct {
    int *A;        // Active buckets
    int *K;        // Key mapping
    int *W;        // Working set
    int *L;        // Location of each bucket in W
    int *R;        // Restore chain (removal parent)
    int N;         // Total buckets (anchor size)
    int a;         // Number of active buckets
} anchor_hash_t;

// Initialize AnchorHash context with N buckets and a active buckets
// Returns 0 on success, -1 on failure
int anchor_hash_init(anchor_hash_t *ctx, int N, int a);

// Free AnchorHash context
void anchor_hash_free(anchor_hash_t *ctx);

// Map a key to a bucket
int anchor_hash_map(anchor_hash_t *ctx, uint64_t key);

// Remove a bucket (simulate node removal)
int anchor_hash_remove(anchor_hash_t *ctx, int b);

// Add a bucket (simulate node addition)
int anchor_hash_add(anchor_hash_t *ctx, int b);

#endif // ANCHOR_HASH_H
