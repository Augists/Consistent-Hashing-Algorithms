#ifndef ANCHOR_HASH_H
#define ANCHOR_HASH_H

#include <stdint.h>

typedef struct anchor_hash_t anchor_hash_t;

anchor_hash_t* anchor_hash_new();
void anchor_hash_free(anchor_hash_t* h);
void anchor_hash_add(anchor_hash_t* h, const char* node);
void anchor_hash_remove(anchor_hash_t* h, const char* node);
const char* anchor_hash_get(anchor_hash_t* h, const char* key);

#endif // ANCHOR_HASH_H
