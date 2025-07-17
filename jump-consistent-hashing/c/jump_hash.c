#include "jump_hash.h"
#include <stdint.h>

// Implementation of Google's Jump Consistent Hash algorithm.
// See paper: "A Fast, Minimal Memory, Consistent Hash Algorithm" by John Lamping and Eric Veach.
int32_t jump_consistent_hash(uint64_t key, int32_t num_buckets) {
    int64_t b = -1, j = 0;

    if (num_buckets <= 0) {
        return -1; // Or handle error appropriately
    }

    while (j < num_buckets) {
        b = j;
        key = key * 2862933555777941757ULL + 1;
        j = (int64_t)(((double)(b + 1)) * ((double)(1LL << 31) / ((double)((key >> 33) + 1))));
    }
    return (int32_t)b;
}