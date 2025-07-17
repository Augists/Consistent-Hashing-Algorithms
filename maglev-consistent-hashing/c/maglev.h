#ifndef MAGLEV_H
#define MAGLEV_H

#include <stddef.h>
#include <stdint.h>

// The main Maglev structure
typedef struct {
    size_t N;      // Number of backend nodes
    size_t M;      // Size of the lookup table
    char** nodes;  // Array of backend node names
    int* lookup;   // The lookup table
} maglev_t;

/**
 * @brief Creates a new Maglev instance.
 *
 * @param nodes An array of strings representing the backend node names.
 * @param N The number of nodes in the array.
 * @param M The desired size of the lookup table. Should be a prime number.
 * @return A pointer to the newly created maglev_t instance, or NULL on failure.
 *
 * The caller is responsible for freeing the returned instance using maglev_free().
 * The `nodes` array is copied, so the caller can free their original array.
 */
maglev_t* maglev_new(const char** nodes, size_t N, size_t M);

/**
 * @brief Frees a Maglev instance and all its associated memory.
 *
 * @param m A pointer to the maglev_t instance to free.
 */
void maglev_free(maglev_t* m);

/**
 * @brief Gets the backend node name for a given key.
 *
 * @param m A pointer to the maglev_t instance.
 * @param key The key to look up.
 * @return A pointer to the node name string. This string is owned by the
 *         maglev_t instance and should not be freed by the caller.
 */
const char* maglev_get(const maglev_t* m, const char* key);

#endif // MAGLEV_H
