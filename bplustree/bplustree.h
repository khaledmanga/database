#pragma once

#include "include/common.h"
#include <vector>

class KV {
  std::vector<u16> key;
  std::vector<u16> value;
}

class BPLusTree {
private:
  u16 type;
  u16 nkeys;
  std::vector<u64> pointers;
  std::vector<u16> offset;
  std::vector<KV> KVs;

public:
  Page encode();
  BPlusTree decode(Page page);
  void update_offset();
};