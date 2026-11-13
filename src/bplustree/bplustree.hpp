#ifndef BPLUSTREE_BPLUSTREE_HPP
#define BPLUSTREE_BPLUSTREE_HPP

#include "include/common.hpp"
#include <vector>

struct KV {
    std::vector<u8> key;
    std::vector<u8> value;
};

struct BPlusTree {
    u16 type;
    u16 nkeys;
    std::vector<u64> pointers;
    std::vector<u16> offsets;
    std::vector<KV> KVs;
};

Page convertBPlusTreeToPage(const BPlusTree& bplustree);
BPlusTree convertPageToBPlusTree(const Page& page);
void update_offset(BPlusTree& bplustree);

#endif