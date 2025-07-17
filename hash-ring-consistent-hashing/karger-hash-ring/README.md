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
