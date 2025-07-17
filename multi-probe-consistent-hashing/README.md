# Multi-Probe Consistent Hashing

This repository contains implementations of the Multi-Probe Consistent Hashing algorithm in Go and C.

## Algorithm Introduction

Multi-Probe Consistent Hashing is an enhancement to the traditional consistent hashing algorithm. While traditional consistent hashing maps a key to a single point on the hash ring and assigns it to the next clockwise node, Multi-Probe Consistent Hashing generates multiple hash points (probes) for a single key. The key is then assigned to the node that is "closest" to any of these probe points on the ring.

**How it works:**

1.  **Virtual Nodes:** Similar to traditional consistent hashing, physical nodes are represented by multiple virtual nodes on the hash ring.
2.  **Multiple Probes:** For each key, instead of generating just one hash, `P` (number of probes) different hashes are generated (e.g., by appending a probe index to the key before hashing: `hash(key + "_probe_0")`, `hash(key + "_probe_1")`, etc.).
3.  **Closest Node Selection:** For each of these `P` probe hashes, the algorithm finds the closest clockwise virtual node on the ring. Among these `P` candidate nodes, the one that is closest to its respective probe point (i.e., has the smallest clockwise distance) is chosen as the final assigned node for the key.

**Key Features:**

*   **Improved Load Distribution:** By probing multiple points, the algorithm can find a "better" (more evenly distributed) node for a key, leading to lower variance in load across servers compared to single-probe consistent hashing.
*   **Reduced Hotspots:** Helps in mitigating hotspots by allowing keys to be assigned to a less loaded server if a nearby probe point leads to it.
*   **Minimal Reassignments:** Retains the core benefit of consistent hashing, minimizing reassignments when nodes are added or removed.

**Trade-offs:**
*   **Increased Lookup Cost:** The lookup time increases by a factor of `P` (number of probes), as multiple lookups on the ring are performed for each key.
*   **Complexity:** Slightly more complex to implement than basic consistent hashing.

## Go Implementation

### Files:

*   `go/multi_probe_hashing.go`: Contains the `MultiProbeHashRing` struct and its methods.
*   `go/multi_probe_hashing_test.go`: Contains basic tests to demonstrate the algorithm's behavior, including observing key distribution changes when nodes are added or removed.

### How to Run Tests:

Navigate to the `go` directory and run the tests:

```bash
cd go
go test -v
```

**Expected Output:**
The test output will show the distribution of keys across nodes before and after adding/removing nodes. You should observe a more even distribution compared to basic consistent hashing with the same number of virtual nodes.

## C Implementation

### Files:

*   `c/multi_probe_hashing.h`: Header file for the Multi-Probe hash ring functions.
*   `c/multi_probe_hashing.c`: Source file containing the Multi-Probe hash ring implementation.
*   `c/test_multi_probe_hashing.c`: Main file with test cases to demonstrate node addition/removal and key re-distribution.
*   `c/Makefile`: Makefile for compiling the C code.

### How to Compile and Run Tests:

Navigate to the `c` directory, compile, and then run the executable:

```bash
cd c
make
./test_multi_probe_hashing
```

**Expected Output:**
Similar to the Go version, the C test output will display key distributions and show the impact of server changes, demonstrating the improved load balancing.
