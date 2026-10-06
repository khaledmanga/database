#pragma once

#include <iostream>
#include <vector>

#include "../common/common.h"

class KV {
  private:
    u16 key;
    u16 value;
};

class BPlusTree {
  private:
    u16 type;
    u16 nkeys;
    std::vector<u64> pointer;
    std::vector<u16> offsets;
    std::vector<KV> KVs;

  public:
    Page encode();
    BPlusTree decode();
};
