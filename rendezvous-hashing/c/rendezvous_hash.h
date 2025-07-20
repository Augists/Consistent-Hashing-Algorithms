#ifndef RENDEZVOUS_HASH_H
#define RENDEZVOUS_HASH_H

#include <stdint.h>
#include <stddef.h> // For size_t

// Function to calculate FNV-1a hash
uint64_t fnv1a_64(const char *s);

// RendezvousHash calculates the highest random weight for a given key and a list of nodes.
// It returns the index of the chosen node.
int rendezvous_hash(const char* key, char** nodes, int num_nodes);

#endif // RENDEZVOUS_HASH_H
