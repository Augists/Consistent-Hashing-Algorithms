#include "anchor_hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

struct anchor_hash_node {
    char* name;
    struct anchor_hash_node* next;
};

struct anchor_hash_t {
    struct anchor_hash_node* nodes;
    size_t count;
};

static uint64_t hash_fnv1a(const char* key) {
    uint64_t hash = FNV_OFFSET_BASIS;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

anchor_hash_t* anchor_hash_new() {
    anchor_hash_t* h = (anchor_hash_t*)malloc(sizeof(anchor_hash_t));
    if (!h) return NULL;
    h->nodes = NULL;
    h->count = 0;
    return h;
}

void anchor_hash_free(anchor_hash_t* h) {
    if (!h) return;
    struct anchor_hash_node* current = h->nodes;
    while (current) {
        struct anchor_hash_node* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    free(h);
}

void anchor_hash_add(anchor_hash_t* h, const char* node) {
    if (!h || !node) return;
    struct anchor_hash_node* new_node = (struct anchor_hash_node*)malloc(sizeof(struct anchor_hash_node));
    if (!new_node) return;
    new_node->name = strdup(node);
    if (!new_node->name) {
        free(new_node);
        return;
    }
    new_node->next = h->nodes;
    h->nodes = new_node;
    h->count++;
}

void anchor_hash_remove(anchor_hash_t* h, const char* node) {
    if (!h || !node || !h->nodes) return;

    struct anchor_hash_node* current = h->nodes;
    struct anchor_hash_node* prev = NULL;

    while (current) {
        if (strcmp(current->name, node) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                h->nodes = current->next;
            }
            free(current->name);
            free(current);
            h->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

const char* anchor_hash_get(anchor_hash_t* h, const char* key) {
    if (!h || !key || h->count == 0) {
        return NULL;
    }

    uint64_t hash = hash_fnv1a(key);
    size_t index = hash % h->count;

    struct anchor_hash_node* current = h->nodes;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current->name;
}
