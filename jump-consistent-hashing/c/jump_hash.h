#ifndef JUMP_HASH_H
#define JUMP_HASH_H

#include <stdint.h>

// jump_consistent_hash computes the bucket number for a given key and number of buckets.
//
// @param key: The 64-bit key to hash.
// @param num_buckets: The number of buckets. Must be greater than 0.
// @return: The bucket number, in the range [0, num_buckets).
int32_t jump_consistent_hash(uint64_t key, int32_t num_buckets);

#endif // JUMP_HASH_H
