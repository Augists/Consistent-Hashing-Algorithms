#ifndef MULTI_PROBE_HASHING_H
#define MULTI_PROBE_HASHING_H

#include <stdint.h>

// Define a struct for the Multi-Probe node
typedef struct {
    uint32_t hash;
    char* node_name;
} MultiProbeNode;

// Define a struct for the Multi-Probe hash ring
typedef struct {
    MultiProbeNode* nodes;        // Sorted array of virtual nodes
    int num_nodes;
    int capacity;
    int replicas;                 // Number of virtual nodes per real node
    int probes;                   // Number of probes to make for a key
} MultiProbeHashRing;

// Function prototypes
MultiProbeHashRing* create_multi_probe_hash_ring(int replicas, int probes);
void destroy_multi_probe_hash_ring(MultiProbeHashRing* hr);
void add_multi_probe_node(MultiProbeHashRing* hr, const char* node_name);
void remove_multi_probe_node(MultiProbeHashRing* hr, const char* node_name);
const char* get_multi_probe_node(MultiProbeHashRing* hr, const char* key);

#endif // MULTI_PROBE_HASHING_H
