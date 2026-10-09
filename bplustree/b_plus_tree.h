#pragma once

#include <iostream>
#include <vector>

#include "../common/common.h"

class KV {
public:
  std::vector<u16> key;
  std::vector<u16> value;
};

class BPlusTree {
public:
  u16 type{};
  u16 nkeys{};
  std::vector<u64> pointers;
  std::vector<u16> offsets;
  std::vector<KV> KVs;

  NodeType typeNode;
  Page encode();
  void update_offset();
  BPlusTree decode(const Page &page);
};
