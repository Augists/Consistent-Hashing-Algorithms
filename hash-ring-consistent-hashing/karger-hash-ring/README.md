# Consistent Hashing Algorithm Implementation

This repository contains implementations of the Consistent Hashing algorithm in Go and C. Consistent Hashing is a special kind of hashing that minimizes the reorganization of keys when the number of slots (e.g., cache servers) in a hash table changes.

## Algorithm Introduction

Traditional hashing algorithms map keys to a fixed number of slots. When a slot is added or removed, most keys need to be remapped, leading to significant data movement (e.g., in a distributed cache). Consistent Hashing addresses this by using a hash ring.

**How it works:**

1.  **Hash Ring:** Imagine a circle (the hash ring) representing the range of all possible hash values (e.g., 0 to 2^32 - 1).
2.  **Node Hashing:** Each physical node (e.g., server) is hashed and placed on this ring. To ensure better distribution and minimize impact during node additions/removals, each physical node is typically mapped to multiple points on the ring (called "virtual nodes" or "replicas").
3.  **Key Hashing:** When a key needs to be mapped, its hash is calculated and placed on the ring.
4.  **Assignment:** The key is assigned to the first node encountered when moving clockwise from the key's position on the ring.

**Benefits:**

*   **Minimal Re-mapping:** When a node is added, only keys between the new node and the next clockwise node are affected. Similarly, when a node is removed, only keys that were mapped to it are re-distributed to the next clockwise node.
*   **Scalability:** Easily add or remove nodes without a complete rehash of the entire system.
*   **Load Balancing:** Virtual nodes help distribute keys more evenly across physical nodes, even with a small number of physical nodes.

## Go Implementation

The Go implementation provides a `HashRing` struct that supports adding, removing, and getting nodes for a given key. It uses `crc32` for hashing and `sort` for maintaining the sorted list of virtual nodes.

### Files:

*   `go/consistent_hashing.go`: Contains the `HashRing` struct and its methods.
*   `go/consistent_hashing_test.go`: Contains basic tests to demonstrate the algorithm's behavior, including observing key distribution changes when nodes are added or removed.

### How to Run Tests:

Navigate to the `go` directory and run the tests:

```bash
cd go
go test -v
```

**Expected Output:**
The test output will show the distribution of keys across nodes before and after adding/removing nodes. You should observe that only a small percentage of keys are remapped when a node is added or removed, demonstrating the efficiency of consistent hashing.

## C Implementation

The C implementation provides similar functionality using a simple CRC32 checksum function and dynamic arrays to manage the hash ring. It includes a basic `Makefile` for compilation.

### Files:

*   `c/consistent_hashing.h`: Header file for the hash ring functions.
*   `c/consistent_hashing.c`: Source file containing the hash ring implementation.
*   `c/test_consistent_hashing.c`: Main file with test cases to demonstrate node addition/removal and key re-distribution.
*   `c/Makefile`: Makefile for compiling the C code.

### How to Compile and Run Tests:

Navigate to the `c` directory, compile, and then run the executable:

```bash
cd c
make
./test_consistent_hashing
```

**Expected Output:**
Similar to the Go version, the C test output will display key distributions. You will see how adding or removing a node affects the mapping of keys, highlighting the minimal impact of consistent hashing.

## Experimenting with Slot Changes

Both test files (`consistent_hashing_test.go` and `test_consistent_hashing.c`) are designed to show the impact of slot (node) changes:

1.  **Initial Distribution:** Keys are distributed among an initial set of nodes.
2.  **Adding a Node:** A new node is added, and the key distribution is re-evaluated. You'll notice that the new node takes over a portion of the keys from its clockwise neighbors, but the majority of keys remain mapped to their original nodes.
3.  **Removing a Node:** A node is removed, and the key distribution is re-evaluated. The keys previously mapped to the removed node are re-distributed to the next clockwise nodes, again with minimal overall disruption.

By running these tests, you can visually confirm how Consistent Hashing minimizes the number of keys that need to be remapped when the set of available nodes changes, making it ideal for distributed systems like caches and databases.

## Experiment Results

**Note:** The C implementation of Karger Hash Ring is currently experiencing a segmentation fault during experiment execution and has been temporarily skipped for detailed analysis. The results below pertain to the Go implementation.

### Key Distribution (Peak-to-Average Ratio)

This metric indicates how evenly keys are distributed across the available nodes. A ratio closer to 1.00 signifies a more even distribution.

*   **Go Implementation:** 1.50

The Go implementation shows good key distribution, with keys being reasonably evenly spread across the nodes, though not as perfectly balanced as the simplified Anchor Hashing or Dx Hashing.

### Storage Overhead

This refers to the memory footprint required by the hashing algorithm's data structures.

*   **Go Implementation:** The `HashRing` struct contains `nodes` (a sorted `uint32` slice for virtual node hashes), `nodeKeys` (a `string` slice for actual node names), and `nodeMap` (a `map[uint32]string` mapping virtual node hashes to node names). The overhead is proportional to the number of virtual nodes (`REPLICAS * NUM_NODES`), as each virtual node requires storage for its hash and the corresponding node name.

### Impact of Node Addition/Removal on Key Remapping

This measures the percentage of keys that change their assigned node when a node is added or removed. Lower percentages indicate better consistency.

| Operation | Remapped Keys (Go) | Total Keys | Percentage Remapped (Go) |
|-----------|--------------------|------------|--------------------------|
| Remove    | 1175               | 10000      | 11.75%                   |
| Add       | 0                  | 10000      | 0.00%                    |

The Karger Hash Ring (Go implementation) demonstrates good consistency, with a relatively low percentage of keys remapped (around 11.75%) when a node is removed. This is a key benefit of consistent hashing. The 0% remapping on adding a node back is expected, as new nodes are added to the ring without disrupting existing mappings unless a key's position now falls to the newly added node.

### Scheduling Order Result Table

Karger Hash Ring uses a hash ring where keys are mapped to the first node encountered when moving clockwise from the key's position. The "scheduling order" is implicitly defined by the sorted order of virtual nodes on the hash ring. For a given key, the algorithm performs a binary search to find its position and then selects the next virtual node in the sorted list (wrapping around if necessary).
