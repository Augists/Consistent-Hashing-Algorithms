#ifndef KETAMA_H
#define KETAMA_H

#include <stdint.h>

// Define a struct for the Ketama node
typedef struct {
    uint32_t hash;
    char* node_name;
} KetamaNode;

// Define a struct for the Ketama hash ring
typedef struct {
    KetamaNode* nodes;        // Sorted array of virtual nodes
    int num_nodes;
    int capacity;
    int num_virtual_nodes;    // Number of virtual nodes per real node
} KetamaRing;

// Function prototypes
KetamaRing* create_ketama_ring(int num_virtual_nodes);
void destroy_ketama_ring(KetamaRing* kr);
void add_ketama_node(KetamaRing* kr, const char* node_name);
void remove_ketama_node(KetamaRing* kr, const char* node_name);
const char* get_ketama_node(KetamaRing* kr, const char* key);

#endif // KETAMA_H
