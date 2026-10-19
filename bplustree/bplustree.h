#pragma once

#include "include/common.h"
#include <vector>

class KV {
  public:
    std::vector<u16> key;
    std::vector<u16> value;
};

class BPlusTree {
private:
  u16 types;
  u16 nkeys;
  std::vector<u64> pointers;
  std::vector<u16> offset;
  std::vector<KV> KVs;

public:
  Page encode();
  void decode(const Page &page);
  void update_offset();
};