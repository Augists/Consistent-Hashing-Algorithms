# Rendezvous Hashing (Highest Random Weight - HRW)

This repository contains implementations of the Rendezvous Hashing (also known as Highest Random Weight or HRW) algorithm in Go and C.

## Algorithm Introduction

Rendezvous Hashing is a consistent hashing algorithm that does not rely on a hash ring. Instead, it directly computes a "weight" for each server (or bucket) for a given key. The key is then assigned to the server with the highest weight.

**How it works:**

For a given key `K` and a set of servers `S = {S1, S2, ..., Sn}`:

1.  For each server `Si`, a pseudo-random weight `W(K, Si)` is calculated. This weight is typically derived from a hash of the key combined with the server's identifier (e.g., `hash(K + Si)`).
2.  The key `K` is assigned to the server `Sj` for which `W(K, Sj)` is the maximum among all servers.

**Key Features:**

*   **Simplicity:** The core algorithm is straightforward to understand and implement.
*   **Decentralized:** No central coordination or shared state is required among servers to determine key assignments.
*   **Minimal Disruption:** When a server is added, it only takes over keys from existing servers if its calculated weight for those keys becomes the new maximum. When a server is removed, its keys are simply reassigned to the next highest-weighted server among the remaining ones. The number of reassignments is minimal.
*   **Good Distribution:** Provides a good, uniform distribution of keys across servers.
*   **No Virtual Nodes:** Unlike traditional hash ring, it doesn't require the concept of virtual nodes, simplifying management.

## Go Implementation

### Files:

*   `go/main.go`: Contains the `RendezvousHash` function and a demonstration of its usage, including observing key distribution changes when nodes are added or removed.

### How to Run:

Navigate to the `go` directory and run the main file:

```bash
cd go
go run main.go
```

**Expected Output:**
The program will show the distribution of keys across servers and demonstrate how adding or removing a server affects key assignments, highlighting the minimal reassignments.

## C Implementation

### Files:

*   `c/main.c`: Contains the `rendezvous_hash` function and a demonstration of its usage.

### How to Compile and Run:

Navigate to the `c` directory, compile, and then run the executable:

```bash
cd c
gcc -o rendezvous_hash main.c
./rendezvous_hash
```

**Expected Output:**
Similar to the Go version, the C program will display key distributions and show the impact of server changes on key assignments.
