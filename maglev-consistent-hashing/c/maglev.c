#include "maglev.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// FNV-1a 64-bit hash
static uint64_t hash_fnv1a_64(const char* key) {
    uint64_t hash = 0xcbf29ce484222325;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(*p);
        hash *= 0x100000001b3;
    }
    return hash;
}

// FNV-1a 32-bit hash
static uint32_t hash_fnv1a_32(const char* key) {
    uint32_t hash = 0x811c9dc5;
    for (const char* p = key; *p; p++) {
        hash ^= (uint32_t)(*p);
        hash *= 0x01000193;
    }
    return hash;
}

// Comparison function for qsort
static int compare_strings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

static void generate_permutations(const maglev_t* m, uint64_t** permutations) {
    for (size_t i = 0; i < m->N; i++) {
        uint64_t offset = hash_fnv1a_64(m->nodes[i]) % m->M;
        uint64_t skip = (hash_fnv1a_32(m->nodes[i]) % (m->M - 1)) + 1;
        permutations[i] = malloc(sizeof(uint64_t) * m->M);
        if (!permutations[i]) return; // Should handle error properly
        for (size_t j = 0; j < m->M; j++) {
            permutations[i][j] = (offset + j * skip) % m->M;
        }
    }
}

static void populate(maglev_t* m) {
    if (m->N == 0) return;

    uint64_t** permutations = malloc(sizeof(uint64_t*) * m->N);
    if (!permutations) return;
    generate_permutations(m, permutations);

    int* next = calloc(m->N, sizeof(int));
    if (!next) {
        // free permutations
        for(size_t i = 0; i < m->N; i++) free(permutations[i]);
        free(permutations);
        return;
    }

    for (size_t i = 0; i < m->M; i++) {
        m->lookup[i] = -1;
    }

    size_t filled_count = 0;
    while (filled_count < m->M) {
        for (size_t i = 0; i < m->N; i++) {
            uint64_t c = permutations[i][next[i]];
            while (m->lookup[c] != -1) {
                next[i]++;
                c = permutations[i][next[i]];
            }
            m->lookup[c] = i;
            next[i]++;
            filled_count++;
            if (filled_count == m->M) {
                break;
            }
        }
    }

    free(next);
    for (size_t i = 0; i < m->N; i++) {
        free(permutations[i]);
    }
    free(permutations);
}

maglev_t* maglev_new(const char** nodes, size_t N, size_t M) {
    if (N == 0 || M == 0) {
        return NULL;
    }

    maglev_t* m = malloc(sizeof(maglev_t));
    if (!m) return NULL;

    m->N = N;
    m->M = M;
    m->nodes = malloc(sizeof(char*) * N);
    if (!m->nodes) {
        free(m);
        return NULL;
    }

    for (size_t i = 0; i < N; i++) {
        m->nodes[i] = strdup(nodes[i]);
        if (!m->nodes[i]) {
            // Cleanup previously allocated memory
            for (size_t j = 0; j < i; j++) free(m->nodes[j]);
            free(m->nodes);
            free(m);
            return NULL;
        }
    }

    // Sort nodes for consistent permutation generation
    qsort(m->nodes, N, sizeof(char*), compare_strings);

    m->lookup = malloc(sizeof(int) * M);
    if (!m->lookup) {
        for (size_t i = 0; i < N; i++) free(m->nodes[i]);
        free(m->nodes);
        free(m);
        return NULL;
    }

    populate(m);

    return m;
}

void maglev_free(maglev_t* m) {
    if (!m) return;
    for (size_t i = 0; i < m->N; i++) {
        free(m->nodes[i]);
    }
    free(m->nodes);
    free(m->lookup);
    free(m);
}

const char* maglev_get(const maglev_t* m, const char* key) {
    uint64_t hash = hash_fnv1a_64(key);
    size_t index = hash % m->M;
    return m->nodes[m->lookup[index]];
}
