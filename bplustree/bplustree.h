#pragma once

#include "include/common.h"
#include <vector>

struct KV {
  std::vector<u16> key;
  std::vector<u16> value;
};

struct BPlusTree {
  u16 types;
  u16 nkeys;
  std::vector<u64> pointers;
  std::vector<u16> offset;
  std::vector<KV> KVs;

  void update_offset();
};

Page encodeBPlusTree(const BPlusTree &bpt);
BPlusTree decodeBPlusTree(const Page &page);