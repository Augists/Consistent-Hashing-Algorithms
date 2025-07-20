package dxhash

import (
  "errors"
)

type Dx struct {
  slots []int
  nodes map[int]struct{}
  k     int
}

func New(capacity int, k int) *Dx {
  s := make([]int, capacity)
  for i := range s { s[i] = -1 }
  return &Dx{slots: s, nodes: make(map[int]struct{}), k: k}
}

func (d *Dx) Add(node int) error {
  if _,ok := d.nodes[node]; ok { return nil }
  d.nodes[node]=struct{}{}
  // 若 load factor > 0.75 则扩展
  if float64(len(d.nodes))/float64(len(d.slots)) > 0.75 { d.expand() }
  // 放置策略：遍历槽找 -1
  placed := 0
  target := len(d.slots)/len(d.nodes)
  for i:=0; i<len(d.slots)&&placed<target; i++ { if d.slots[i]==-1 { d.slots[i]=node; placed++ } }
  return nil
}

func (d *Dx) expand() {
  old := d.slots
  d.slots = make([]int, len(old)*2)
  for i := range d.slots { d.slots[i] = -1 }
  // 重新散列 (最小化迁移：只将旧元素按相同索引或双倍偏移复制)
  for i,v := range old { if v!=-1 { d.slots[i]=v } }
}

func (d *Dx) Remove(node int) error {
  if _,ok := d.nodes[node]; !ok { return errors.New("notfound") }
  delete(d.nodes,node)
  // 标记 -> -1 (保持最小影响)
  for i,v := range d.slots { if v==node { d.slots[i]=-1 } }
  return nil
}

func (d *Dx) Get(key []byte) int {
  // 生成 k 候选
  for i:=0; i<d.k; i++ {
     h := hash64(key, uint64(i)) % uint64(len(d.slots))
     if d.slots[h] != -1 { return d.slots[h] }
  }
  return -1 // 未命中
}
