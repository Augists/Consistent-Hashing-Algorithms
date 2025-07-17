# Jump Consistent Hash

This repository contains implementations of the Jump Consistent Hash algorithm in Go and C.

## Algorithm Introduction

Jump Consistent Hash is a fast, minimal-disruption consistent hash algorithm designed by Google. Unlike traditional consistent hashing which uses a hash ring with virtual nodes, Jump Consistent Hash provides a simpler and faster way to map keys to buckets (servers) with minimal reassignments when the number of buckets changes.

**How it works:**

The algorithm takes a 64-bit key and the number of buckets as input. It iteratively calculates a bucket index using a series of "jumps". The core idea is that when the number of buckets increases, only a small fraction of keys (approximately `1/new_buckets`) need to be remapped.

**Key Features:**

*   **Simplicity:** The algorithm is very simple and compact, making it easy to implement.
*   **Speed:** It's extremely fast, as it avoids complex data structures like hash rings and relies on simple arithmetic operations.
*   **Minimal Disruption:** When the number of buckets changes, it minimizes the number of keys that are remapped, similar to traditional consistent hashing but with a different approach.
*   **No State:** The algorithm is stateless; it doesn't require maintaining any information about the buckets or their positions.

## Go Implementation

### Files:

*   `go/main.go`: Contains the `JumpConsistentHash` function and a simple example demonstrating its usage.

### How to Run:

Navigate to the `go` directory and run the main file:

```bash
cd go
go run main.go
```

**Expected Output:**
The program will demonstrate how different keys are mapped to buckets and how the mapping changes when the number of buckets increases, showing the minimal reassignments.

## C Implementation

### Files:

*   `c/main.c`: Contains the `jump_consistent_hash` function and a simple example demonstrating its usage.

### How to Compile and Run:

Navigate to the `c` directory, compile, and then run the executable:

```bash
cd c
gcc -o jump_hash main.c
./jump_hash
```

**Expected Output:**
Similar to the Go version, the C program will show key-to-bucket mappings and how they are affected by changes in the number of buckets.
