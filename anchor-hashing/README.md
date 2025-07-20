# Anchor Hashing

## AnchorHash 原理(核心思想)

AnchorHash 维护：

1. 固定大小的 锚集合 A (容量 M ≥ 当前节点数 N)。
2. 动态 活动集合 W (当前在线节点)。
3. 一个 退役堆栈 R，用于快速回收和再激活。

映射过程：对 key 迭代哈希：

```c
h = H(key) % |A|
while bucket h 不在 W:
    h = H(key || h) % prev_removed_index_of(h)
return bucket_to_node[h]
```

通过可逆的剥离顺序确保删除节点时仅影响该节点键的最少子集；添加时 O(1) 恢复。其关键是：每个被删除的 bucket 记录一个“前驱活动容量”值，用于限制下一轮哈希范围，保证期望常数次重试。

性质：

- 查找期望 O(1) (摊还)
- 删除 / 添加 O(1)
- 重映射最小：仅涉及被删 bucket 的键。

## AnchorHash C 实现 (anchorhash.h / anchorhash.c)

核心结构：

```c
typedef struct anchorhash {
    ch_mapping_if_t vtbl;
    int M;              // 锚总数
    int active;         // 当前活动数
    int *bucket2node;   // 大小 M
    int *node_refcnt;   // 统计 (可选)
    int *stack;         // 退役栈 (存放被删除 bucket 序列)
    int top;            // 栈顶
    int *prev_size;     // 每个 bucket 删除当时的活动容量
    uint64_t seed;
} anchorhash_t;
```

get_node 伪代码：

```c
int anchor_get_node(void *self, const void *key, size_t len) {
    anchorhash_t *ctx = self;
    uint64_t h = ch_hash64(key, len, ctx->seed);
    int b = h % ctx->M;
    while (ctx->bucket2node[b] == -1) {
        uint64_t h2 = ch_hash64(&h, sizeof(h), ctx->seed ^ b);
        int limit = ctx->prev_size[b];
        b = h2 % limit;
        h = h2;
    }
    return ctx->bucket2node[b];
}
```

删除：将该 bucket 标记为 -1，记录删除时活动数，入栈；添加：出栈恢复。
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
