#include "dx_hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

struct dx_hash_t {
    char** nodes;
    int* node_status;
    int* inactive;
    size_t size;
    size_t inactive_count;
};

static uint64_t hash_fnv1a(const char* key) {
    uint64_t hash = FNV_OFFSET_BASIS;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

static uint64_t next_random(uint64_t r) {
    return (r * 1664525 + 1013904223);
}

dx_hash_t* dx_hash_new(size_t size) {
    dx_hash_t* h = (dx_hash_t*)malloc(sizeof(dx_hash_t));
    if (!h) return NULL;

    h->nodes = (char**)calloc(size, sizeof(char*));
    h->node_status = (int*)calloc(size, sizeof(int));
    h->inactive = (int*)malloc(size * sizeof(int));
    h->size = size;
    h->inactive_count = size;

    for (size_t i = 0; i < size; i++) {
        h->inactive[i] = size - 1 - i;
    }

    return h;
}

void dx_hash_free(dx_hash_t* h) {
    if (!h) return;
    for (size_t i = 0; i < h->size; i++) {
        free(h->nodes[i]);
    }
    free(h->nodes);
    free(h->node_status);
    free(h->inactive);
    free(h);
}

int dx_hash_add(dx_hash_t* h, const char* node) {
    if (h->inactive_count == 0) {
        return -1; // Hash is full
    }

    int index = h->inactive[--h->inactive_count];
    h->nodes[index] = strdup(node);
    h->node_status[index] = 1;

    return index;
}

int dx_hash_remove(dx_hash_t* h, const char* node) {
    for (size_t i = 0; i < h->size; i++) {
        if (h->node_status[i] && strcmp(h->nodes[i], node) == 0) {
            h->node_status[i] = 0;
            h->inactive[h->inactive_count++] = i;
            free(h->nodes[i]);
            h->nodes[i] = NULL;
            return 0; // Success
        }
    }
    return -1; // Node not found
}

const char* dx_hash_get(dx_hash_t* h, const char* key) {
    if (h->inactive_count == h->size) {
        return NULL; // No nodes in hash
    }

    uint64_t r = hash_fnv1a(key);
    while (1) {
        size_t node_id = r % h->size;
        if (h->node_status[node_id]) {
            return h->nodes[node_id];
        }
        r = next_random(r);
    }
}
