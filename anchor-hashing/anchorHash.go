package anchorhash

import (
  "encoding/binary"
  "hash/fnv"
)

type Anchor struct {
    bucket2node []int
    prevSize    []int
    stack       []int
    top         int
    active      int
}

func New(max int) *Anchor {
    b2n := make([]int, max)
    for i := range b2n { b2n[i] = -1 }
    return &Anchor{bucket2node: b2n, prevSize: make([]int, max), stack: make([]int, 0, max)}
}

func (a *Anchor) Add(node int) error {
    // reuse from stack or first -1
    var b int
    if a.top > 0 { // has retired
        a.top--
        b = a.stack[a.top]
    } else {
        // linear search; 可优化维护空闲列表
        for i,v := range a.bucket2node { if v==-1 { b=i; goto FOUND } }
        return fmt.Errorf("no capacity")
    }
FOUND:
    a.bucket2node[b] = node
    a.active++
    return nil
}

func (a *Anchor) Remove(node int) error {
    // 简化：线性找到 bucket
    for b, v := range a.bucket2node { if v==node { a.bucket2node[b]=-1; a.prevSize[b]=a.active; a.stack = append(a.stack, b); a.top++; a.active--; return nil }}
    return fmt.Errorf("node not found")
}

func (a *Anchor) Get(key []byte) int {
    h := hash64(key, 0)
    b := int(h % uint64(len(a.bucket2node)))
    for a.bucket2node[b] == -1 {
        h = hash64Uint64(h, uint64(b))
        limit := a.prevSize[b]
        if limit == 0 { return -1 }
        b = int(h % uint64(limit))
    }
    return a.bucket2node[b]
}
