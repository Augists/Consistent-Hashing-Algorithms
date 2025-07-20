#include <stdio.h>
#include <stdlib.h>
#include "jump_hash.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <key> <num_buckets>\n", argv[0]);
        return 1;
    }
    uint64_t key = strtoull(argv[1], NULL, 10);
    int32_t num_buckets = atoi(argv[2]);
    if (num_buckets <= 0) {
        fprintf(stderr, "Number of buckets must be positive.\n");
        return 1;
    }
    int32_t bucket = jump_consistent_hash(key, num_buckets);
    printf("Key %llu maps to bucket %d\n", (unsigned long long)key, bucket);
    return 0;
}
