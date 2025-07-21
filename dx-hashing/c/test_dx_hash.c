#include "dx_hash.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

void test_basic_mapping() {
    dx_hash_t ctx;
    assert(dx_hash_init(&ctx, 10, 10) == 0);
    for (uint64_t k = 0; k < 1000; ++k) {
        int b = dx_hash_map(&ctx, k);
        assert(b >= 0 && b < 10);
    }
    dx_hash_free(&ctx);
    printf("test_basic_mapping passed\n");
}

void test_remove_add() {
    dx_hash_t ctx;
    dx_hash_init(&ctx, 8, 8);
    int before = dx_hash_map(&ctx, 12345);
    dx_hash_remove(&ctx, before);
    int after = dx_hash_map(&ctx, 12345);
    assert(after != before);
    dx_hash_add(&ctx, before);
    int again = dx_hash_map(&ctx, 12345);
    assert(again == before);
    dx_hash_free(&ctx);
    printf("test_remove_add passed\n");
}

int main() {
    test_basic_mapping();
    test_remove_add();
    printf("All DxHash C tests passed!\n");
    return 0;
}
