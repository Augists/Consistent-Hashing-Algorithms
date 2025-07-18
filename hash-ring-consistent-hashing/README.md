# Hash Ring Consistent Hashing Variants

This directory contains implementations of two prominent consistent hashing algorithms that are based on the concept of a hash ring: **Traditional Hash Ring (Karger's)** and **Ketama Hashing**.

Both algorithms aim to distribute keys across a set of nodes (servers) while minimizing reassignments when nodes are added or removed. However, they differ in their specific approaches to virtual node placement and hashing.

## 1. Traditional Hash Ring (Karger's Consistent Hashing)

This is the foundational consistent hashing algorithm, often associated with the work of Karger et al. It maps both nodes and keys onto a conceptual ring (typically a 32-bit or 64-bit integer space). To achieve better load distribution and reduce variance, each physical node is represented by multiple "virtual nodes" (or replicas) on the ring.

**Key Characteristics:**
*   **Virtual Nodes:** Each physical node is assigned a configurable number of virtual nodes. These virtual nodes are typically generated by hashing combinations of the node's name and an index (e.g., `hash(node_name + i)`).
*   **Key Assignment:** A key is hashed to a point on the ring. It is then assigned to the first virtual node encountered when moving clockwise from the key's position.
*   **Load Balancing:** The number of virtual nodes directly impacts the uniformity of load distribution. More virtual nodes generally lead to better balance but increase the ring's size and lookup complexity.

**Implementations:**
*   **Go:** `karger-hash-ring/go/consistent_hashing.go`
*   **C:** `karger-hash-ring/c/consistent_hashing.c`

**How to Run Tests:** Refer to the `karger-hash-ring/README.md` for detailed instructions.

## 2. Ketama Hashing

Ketama Hashing is a specialized and widely adopted implementation of consistent hashing, famously used in Memcached. While it also uses a hash ring, it has specific conventions for hashing and virtual node generation that distinguish it from a generic hash ring implementation.

**Key Characteristics:**
*   **MD5 Hashing:** Ketama explicitly uses the MD5 hash function for both server names and keys. This is a fixed choice, unlike generic hash rings which can use various hash functions.
*   **Specific Virtual Node Generation:** For each physical server, Ketama typically generates a large, fixed number of virtual nodes (e.g., 160 virtual nodes per server). These are derived by taking the 128-bit MD5 hash of `server_name + i` (where `i` is an index) and interpreting the 16-byte MD5 digest as four 32-bit integers, each representing a virtual node on the ring. This precise method aims for very uniform distribution.
*   **Proven in Production:** Its widespread use in Memcached attests to its robustness and effectiveness in distributed caching scenarios.

**Implementations:**
*   **Go:** `ketama-hashing/go/ketama.go`
*   **C:** `ketama-hashing/c/ketama.c`

**How to Run Tests:** Refer to the `ketama-hashing/README.md` for detailed instructions.

## Key Differences and Relationship

*   **Relationship:** Ketama Hashing can be considered a **specific variant or optimized implementation** of the Traditional Hash Ring concept. It takes the general idea of a hash ring and applies particular choices for hashing (MD5) and virtual node distribution to achieve highly predictable and uniform behavior, especially for caching systems.
*   **Hashing Function:** Traditional Hash Ring implementations are flexible with hash functions, while Ketama is tied to MD5.
*   **Virtual Node Density/Placement:** Ketama has a very high and specific density of virtual nodes per server (e.g., 160), generated in a deterministic way from MD5 outputs. Generic hash rings allow more flexibility in the number and generation of virtual nodes.
*   **Use Case Focus:** Ketama is highly optimized and widely adopted for distributed caching, whereas the Traditional Hash Ring is a more general-purpose consistent hashing approach applicable to various distributed system problems.
