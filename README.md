# Consistent Hashing Algorithms

This repository explores different consistent hashing algorithms, providing implementations in Go and C for each. Consistent hashing is a crucial technique in distributed systems for distributing data or requests across a dynamic set of servers (nodes) while minimizing data movement when nodes are added or removed.

## Algorithms Covered:

1.  **Hash Ring Consistent Hashing Variants**
2.  **Jump Consistent Hash**
3.  **Maglev Consistent Hash**
4.  **Rendezvous Hashing (HRW - Highest Random Weight)**
5.  **Multi-Probe Consistent Hashing**

## 1. Hash Ring Consistent Hashing Variants

This section covers consistent hashing algorithms that are based on the concept of a hash ring, including the **Traditional Hash Ring (Karger's)** and **Ketama Hashing**. While both use a ring structure, they differ in their specific implementation details, particularly regarding virtual node placement and hashing functions.

For detailed information on these variants, their implementations, and how to run their tests, please refer to the `hash-ring-consistent-hashing/README.md` file.

## 2. Jump Consistent Hash

**Concept:** Developed by Google, Jump Consistent Hash is a simpler, faster, and stateless algorithm. It maps a 64-bit key to an integer in the range `[0, num_buckets)` using a series of "jumps". Its elegance lies in its mathematical properties that ensure minimal reassignments when `num_buckets` increases.

**Pros:**
*   Extremely simple and compact implementation.
*   Very fast, as it avoids complex data structures.
*   Stateless, requiring no knowledge of existing buckets.
*   Guarantees minimal reassignments when buckets are added.

**Cons:**
*   Only supports increasing the number of buckets; removing buckets is not directly supported in a consistent manner without remapping.
*   Less flexible for custom weighting of nodes compared to hash ring.

**Implementations:**
*   **Go:** `jump-consistent-hashing/go/main.go`
*   **C:** `jump-consistent-hashing/c/main.c`

**How to Run:** Refer to the `jump-consistent-hashing/README.md` for detailed instructions.

## 3. Maglev Consistent Hash

**Concept:** Also from Google, Maglev Consistent Hash is designed for high-performance software load balancers. It uses a lookup table (permutation table) for each backend, which is pre-computed. When a key arrives, its hash is used to index into a master permutation table to find the assigned backend. This allows for very fast lookups (O(1)) and graceful handling of backend failures.

**Pros:**
*   Extremely fast lookups (O(1)).
*   Excellent load balancing and minimal disruption on backend changes.
*   Handles backend failures gracefully.
*   Supports weighted backends.

**Cons:**
*   More complex to implement than Jump Consistent Hash.
*   Requires pre-computation of permutation tables, which can be resource-intensive for very large numbers of backends.

**Implementations:**
*   **Go:** `maglev-consistent-hashing/go/`
*   **C:** `maglev-consistent-hashing/c/`

**How to Run:** Refer to the `maglev-consistent-hashing/README.md` for detailed instructions within each language directory.

## 4. Rendezvous Hashing (HRW - Highest Random Weight)

**Concept:** Rendezvous Hashing (also known as Highest Random Weight or HRW) is a consistent hashing algorithm that does not rely on a hash ring. Instead, for a given key, it calculates a "weight" for each server (or bucket) by combining the key and the server identifier (e.g., `hash(key + server_id)`). The key is then assigned to the server with the highest calculated weight.

**Pros:**
*   Simple and decentralized; no central coordination or shared state needed.
*   Minimal reassignments on node additions or removals.
*   Provides good, uniform distribution of keys.
*   No virtual nodes required, simplifying management.

**Cons:**
*   Lookup time is O(N) where N is the number of nodes, as it needs to iterate through all nodes to find the highest weight. This can be a bottleneck for a very large number of nodes.
*   Less flexible for custom weighting of nodes compared to hash ring or Maglev.

**Implementations:**
*   **Go:** `rendezvous-hashing/go/main.go`
*   **C:** `rendezvous-hashing/c/main.c`

**How to Run:** Refer to the `rendezvous-hashing/README.md` for detailed instructions.

## 5. Multi-Probe Consistent Hashing

**Concept:** Multi-Probe Consistent Hashing is an enhancement to the traditional consistent hashing algorithm. While traditional consistent hashing maps a key to a single point on the hash ring and assigns it to the next clockwise node, Multi-Probe Consistent Hashing generates multiple hash points (probes) for a single key. The key is then assigned to the node that is "closest" to any of these probe points on the ring.

**Pros:**
*   Improved Load Distribution: By probing multiple points, the algorithm can find a "better" (more evenly distributed) node for a key, leading to lower variance in load across servers compared to single-probe consistent hashing.
*   Reduced Hotspots: Helps in mitigating hotspots by allowing keys to be assigned to a less loaded server if a nearby probe point leads to it.
*   Minimal Reassignments: Retains the core benefit of consistent hashing, minimizing reassignments when nodes are added or removed.

**Cons:**
*   Increased Lookup Cost: The lookup time increases by a factor of P (number of probes), as multiple lookups on the ring are performed for each key.
*   Complexity: Slightly more complex to implement than basic consistent hashing.

**Implementations:**
*   **Go:** `multi-probe-consistent-hashing/go/multi_probe_hashing.go`
*   **C:** `multi-probe-consistent-hashing/c/multi_probe_hashing.c`

**How to Run:** Refer to the `multi-probe-consistent-hashing/README.md` for detailed instructions.

| Feature             | Hash Ring Variants          | Jump Consistent Hash      | Maglev Consistent Hash      | Rendezvous Hashing (HRW)    | Multi-Probe Consistent Hashing |
| :------------------ | :-------------------------- | :------------------------ | :-------------------------- | :-------------------------- | :----------------------------- |
| **Complexity**      | Medium                      | Low                       | High                        | Low                         | Medium                         |
| **Lookup Speed**    | O(log N)                    | O(log N) (iterative)      | O(1)                        | O(N)                        | O(P * log N)                   |
| **Node Add/Remove** | Good (minimal reassignments)| Good (add only)           | Excellent                   | Good (minimal reassignments)| Good (minimal reassignments)   |
| **Stateful?**       | Yes (maintains ring)        | No                        | Yes (permutation tables)    | No                          | Yes (maintains ring)           |
| **Weighted Nodes**  | Yes (via virtual nodes)     | No                        | Yes                         | Yes (via custom weight func)| Yes (via virtual nodes)        |
| **Use Case**        | General distributed systems, caching | Simple, fast key mapping  | High-performance load balancing | Simple, decentralized key mapping | Improved load balancing for hash rings |

Choose the algorithm that best fits your specific requirements for complexity, performance, and flexibility in handling node changes.
