#include <stdio.h>
#include <assert.h>
#include "jump_hash.h"

void test_basic() {
    assert(jump_consistent_hash(1, 1) == 0);
    assert(jump_consistent_hash(1, 10) == 6);
    assert(jump_consistent_hash(1, 100) == 55);
    assert(jump_consistent_hash(2, 100) == 62);
    assert(jump_consistent_hash(0xdeadbeef, 1000) == 285);
    printf("test_basic passed.\n");
}

// Test that for a given key, as the number of buckets increases,
// the bucket assignment is consistent.
void test_consistency() {
    uint64_t key = 12345;
    int32_t last_bucket = jump_consistent_hash(key, 1);
    assert(last_bucket == 0);
    for (int32_t i = 2; i < 1000; i++) {
        int32_t bucket = jump_consistent_hash(key, i);
        assert(bucket >= 0 && bucket < i);
        // The bucket should be either the same as before or a new bucket.
        // With jump consistent hash, the destination for a key k can only change
        // from bucket b to bucket n when the number of buckets is increased from n to n+1.
        int32_t previous_bucket = jump_consistent_hash(key, i - 1);
        assert(bucket == previous_bucket || bucket == i - 1);
    }
    printf("test_consistency passed.\n");
}

int main() {
    test_basic();
    test_consistency();
    printf("All C tests passed!\n");
    return 0;
}
