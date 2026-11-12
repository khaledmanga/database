#ifndef BPLUSTREE_BPLUSTREE_HPP
#define

#include <inlcude/common.h>
#include <vector>

struct KV {
    std::vector<u16> key;
    std::vector<u16> value;
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

#endif