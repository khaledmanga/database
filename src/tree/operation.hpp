#ifndef TREE_OPERATION_HPP
#define TREE_OPERATION_HPP

#include "bplustree/bplustree.hpp"
#include "include/common.hpp"
#include "storage/page.hpp"
#include <vector>

struct RecursiveInsert {
  u8 right_page_id = 0;
  std::vector<u8> separator_key;
};

void insert(u8 &root_page_id, const KV &kv);
RecursiveInsert insertRecursive(u8 page_id, const KV &kv);

#endif