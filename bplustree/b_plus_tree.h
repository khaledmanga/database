#pragma once

#include <iostream>
#include <vector>

#include "../common/common.h"

class KV {
  private:
    std::vector<u16> key;
    std::vector<u16> value;
};

class BPlusTree {
  private:
    u16 type;
    u16 nkeys;
    std::vector<u64> pointers;
    std::vector<u16> offsets;
    std::vector<KV> KVs;

  public:
    Page encode();
    BPlusTree decode();
};
