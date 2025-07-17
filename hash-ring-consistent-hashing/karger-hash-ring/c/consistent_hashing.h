#ifndef CONSISTENT_HASHING_H
#define CONSISTENT_HASHING_H

#include <stdint.h>

// Define a struct for the hash ring
typedef struct {
    uint32_t* nodes;        // Sorted array of virtual node hashes
    char** node_names;      // Array of corresponding node names
    int num_nodes;
    int capacity;
    int replicas;           // Number of virtual nodes per real node
} HashRing;

// Function prototypes
HashRing* create_hash_ring(int replicas);
void destroy_hash_ring(HashRing* hr);
void add_node(HashRing* hr, const char* node_name);
void remove_node(HashRing* hr, const char* node_name);
const char* get_node(HashRing* hr, const char* key);

#endif // CONSISTENT_HASHING_H
