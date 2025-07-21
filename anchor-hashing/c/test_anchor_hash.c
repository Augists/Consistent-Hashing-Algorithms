#include "anchor_hash.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

void test_basic_mapping() {
    anchor_hash_t ctx;
    assert(anchor_hash_init(&ctx, 10, 10) == 0);
    for (uint64_t k = 0; k < 1000; ++k) {
        int b = anchor_hash_map(&ctx, k);
        assert(b >= 0 && b < 10);
    }
    anchor_hash_free(&ctx);
    printf("test_basic_mapping passed\n");
}

void test_remove_add() {
    anchor_hash_t ctx;
    anchor_hash_init(&ctx, 8, 8);
    int before = anchor_hash_map(&ctx, 12345);
    anchor_hash_remove(&ctx, before);
    int after = anchor_hash_map(&ctx, 12345);
    assert(after != before);
    anchor_hash_add(&ctx, before);
    int again = anchor_hash_map(&ctx, 12345);
    assert(again == before);
    anchor_hash_free(&ctx);
    printf("test_remove_add passed\n");
}

int main() {
    test_basic_mapping();
    test_remove_add();
    printf("All AnchorHash C tests passed!\n");
    return 0;
}
