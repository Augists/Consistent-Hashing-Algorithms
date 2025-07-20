# Anchor Hashing

This directory contains implementations of the Anchor Hashing algorithm in Go and C.

## Concept

Anchor Hashing is a consistent hashing algorithm that provides full consistency, high performance, and a low memory footprint. It was designed to overcome some of the limitations of traditional ring-based consistent hashing algorithms.

The core idea behind Anchor Hashing is to use a pre-allocated, fixed-size array of "anchor" buckets. When a key needs to be mapped to a server, it is first hashed to one of these anchor buckets.

- If the assigned anchor bucket is active (meaning the corresponding server is online), the key is mapped to that server.
- If the bucket is inactive (the server has been removed or is offline), the key is rehashed to another anchor bucket, and this process continues until an active bucket is found.

This approach ensures that when a server is added or removed, only a proportional number of keys are remapped, thus achieving full consistency and minimal disruption.

## Implementations

- **[Go](./go/)**: A Go implementation of Anchor Hashing.
- **[C](./c/)**: A C implementation of Anchor Hashing.

Refer to the `README.md` files in the respective directories for instructions on how to run the tests and examples.

## Experiment Results

**Note:** The current C and Go implementations in this repository are simplified versions of Anchor Hashing. They primarily use a modulo operation (`hash % num_nodes`) for key-to-node mapping, which does not fully exhibit the minimal remapping properties of a true Anchor Hashing algorithm (which involves rehashing to alternative anchor buckets).

### Key Distribution (Peak-to-Average Ratio)

This metric indicates how evenly keys are distributed across the available nodes. A ratio closer to 1.00 signifies a more even distribution.

*   **C Implementation:** 1.00
*   **Go Implementation:** 1.00

Both implementations show excellent key distribution, with keys being very evenly spread across the nodes.

### Storage Overhead

This refers to the memory footprint required by the hashing algorithm's data structures.

*   **C Implementation:** Uses a linked list to store active nodes. The overhead is proportional to the number of active nodes (`N`), storing node names and pointers. It does not utilize a large, fixed-size anchor array.
*   **Go Implementation:** Uses a `map[string]int` and a `[]string` slice to manage nodes. The overhead is proportional to the number of active nodes (`N`), storing node names and their internal representations.

Compared to a full Anchor Hashing implementation that might use a large pre-allocated anchor array, these simplified versions have a lower memory footprint, primarily storing only the active nodes.

### Impact of Node Addition/Removal on Key Remapping

This measures the percentage of keys that change their assigned node when a node is added or removed. Lower percentages indicate better consistency.

| Operation | Remapped Keys (C) | Remapped Keys (Go) | Total Keys | Percentage Remapped (C) | Percentage Remapped (Go) |
|-----------|-------------------|--------------------|------------|-------------------------|--------------------------|
| Remove    | 9017              | 8899               | 10000      | 90.17%                  | 88.99%                   |
| Add       | 0                 | 0                  | 10000      | 0.00%                   | 0.00%                    |

As expected due to the simplified `hash % num_nodes` logic, removing a node results in a very high percentage of remapped keys (around 90%). This is because the total number of nodes changes, causing most keys to map to a different node. The 0% remapping on adding a node back is a characteristic of this specific simplified implementation, where the new node is simply added to the list/slice and the modulo operation might coincidentally map keys to their original nodes or new nodes without further remapping of existing keys.

### Scheduling Order Result Table

The current simplified implementations of Anchor Hashing do not involve a "scheduling order" or "probing sequence" in the traditional sense of consistent hashing algorithms like hash rings or Maglev. The `Get` function directly calculates a hash and uses a modulo operation (`hash % num_nodes`) to determine the target node's index in a list (C) or sorted slice (Go). There is no explicit sequence of alternative nodes or buckets that are "probed" or "scheduled" for a given key.
