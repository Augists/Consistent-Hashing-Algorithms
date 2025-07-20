# Dx Hashing

## DxHash 原理(核心思想)

DxHash 通过构造多层 deterministic 混合散列结构 (类似跳表式 / 分段式双指数分割) 来实现：

- 无需大规模虚拟节点
- 近似均匀分布
- 快速调整：当节点数变化时，使用分段/对数层次结构重新定位受影响 segment，使重映射接近最小。

一般实现抓住两个要点：

1. 多个不同哈希函数 H_i(key) 生成候选槽位。
2. 利用层次化位运算/指数扩展策略 (doubling / x-split) 在 N 变化时只部分扩展映射表，保持过去分配的绝大多数槽位稳定。

## DxHash C 实现 (dxhash.h / dxhash.c)

为示例，采用多候选 + 级联表：

```c
#define DX_K 3   // 候选数

typedef struct dxhash {
    ch_mapping_if_t vtbl;
    int capacity;      // 当前容量 (按 2^m 扩展)
    int nodes;         // 活动节点数
    int *slots;        // 长度 capacity, 值为 node_id
    uint64_t seed;
} dxhash_t;
```

添加节点：

1. 若 nodes == capacity，扩展 capacity *= 2，重建新段 (只对新增一半区间初始化)。
2. 将新 node_id 均匀填入若干空槽 (或最不平衡槽替换策略)。

查找：生成 K 个哈希 h_i = H_i(key) % capacity，选择第一个非 -1 的槽；若冲突策略可选“最少负载”统计。

> 说明：此 DxHash 简化版用于演示；真实 DxHash 可能有更精细的层次映射与 minimal remap 证明，可在 README 中说明“该实现基于原理重构的工程近似版”。

This directory contains implementations of the Dx Hashing algorithm in Go and C.

## Concept

Dx Hashing is a modern, scalable consistent hashing algorithm designed for efficiency and minimal disruption in distributed systems. Its key innovation is the use of a pseudo-random sequence to map keys to their designated nodes.

The working principle is as follows:

1.  **Initialization**: The cluster is initialized with a fixed maximum number of nodes, and their states (active/inactive) are tracked in an array.
2.  **Key-to-Sequence**: When a key needs to be mapped, it is hashed to generate a seed for a pseudo-random number generator (PRNG).
3.  **Iterative Search**: The PRNG produces a deterministic sequence of node IDs. The algorithm iterates through this sequence, checking the status of each generated node ID.
4.  **First Active Node**: The first node ID in the sequence that corresponds to an *active* node is chosen as the destination for the key.

This method provides excellent load balancing and allows for extremely fast (O(1)) node additions and removals.

## Implementations

- **[Go](./go/)**: A Go implementation of Dx Hashing.
- **[C](./c/)**: A C implementation of Dx Hashing.

Refer to the `README.md` files in the respective directories for instructions on how to run the tests and examples.

## Experiment Results

### Key Distribution (Peak-to-Average Ratio)

This metric indicates how evenly keys are distributed across the available nodes. A ratio closer to 1.00 signifies a more even distribution.

*   **C Implementation:** 1.12
*   **Go Implementation:** 1.12

Both implementations show good key distribution, with keys being reasonably evenly spread across the nodes.

### Storage Overhead

This refers to the memory footprint required by the hashing algorithm's data structures.

*   **C Implementation:** Uses `char** nodes` (array of node names), `int* node_status` (array for active/inactive status), and `int* inactive` (array for inactive indices). The primary overhead is determined by the fixed `HASH_SIZE` (20 in experiments), which pre-allocates space for a maximum number of nodes, plus the storage for actual node names.
*   **Go Implementation:** Uses `[]string nodes` (slice of node names), `[]bool nodeStatus` (slice for active/inactive status), and `[]int inactiveIndices` (slice for inactive indices). Similar to C, the overhead is primarily dictated by the `hashSize` (20 in experiments) and the storage for node names.

Dx Hashing's storage overhead is proportional to its configured `HASH_SIZE` (maximum number of nodes it can support) rather than just the current number of active nodes. This allows for O(1) add/remove operations but means a fixed memory commitment regardless of current cluster size.

### Impact of Node Addition/Removal on Key Remapping

This measures the percentage of keys that change their assigned node when a node is added or removed. Lower percentages indicate better consistency.

| Operation | Remapped Keys (C) | Remapped Keys (Go) | Total Keys | Percentage Remapped (C) | Percentage Remapped (Go) |
|-----------|-------------------|--------------------|------------|-------------------------|--------------------------|
| Remove    | 1122              | 1122               | 10000      | 11.22%                  | 11.22%                   |
| Add       | 0                 | 0                  | 10000      | 0.00%                   | 0.00%                    |

Dx Hashing demonstrates good consistency, with a relatively low percentage of keys remapped (around 11.22%) when a node is removed. This is significantly better than simple modulo hashing. The 0% remapping on adding a node back is a characteristic of this algorithm, as new nodes are added to available slots without disrupting existing mappings unless a key's pseudo-random sequence now points to the newly added node as its first active choice.

### Scheduling Order Result Table

Dx Hashing's "scheduling order" is inherent in its iterative search mechanism. For each key, a pseudo-random number generator (PRNG) produces a deterministic sequence of node IDs. The algorithm then iterates through this sequence, checking the status of each generated node ID. The "order" is the sequence generated by the PRNG for a given key. It's not a fixed global table but a per-key deterministic sequence.
