#pragma once

#include "../bplustree/b_plus_tree.h"
#include "../storage/metadata.h"
#include "../storage/page.h"
#include <optional>
#include <vector>

u64 find_leaf(const std::vector<u16> &key, Pager &pager, u64 page_id);
void insert(const KV &kv, Pager &pager);
std::optional<BPlusTree> search(const KV &kv, Pager &pager);
