#ifndef DX_HASH_H
#define DX_HASH_H

#include <stdint.h>
#include <stddef.h>

typedef struct dx_hash_t dx_hash_t;

dx_hash_t* dx_hash_new(size_t size);
void dx_hash_free(dx_hash_t* h);
int dx_hash_add(dx_hash_t* h, const char* node);
int dx_hash_remove(dx_hash_t* h, const char* node);
const char* dx_hash_get(dx_hash_t* h, const char* key);

#endif // DX_HASH_H
