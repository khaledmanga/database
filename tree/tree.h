#pragma once

#include "bplustree/bplustree.h"
#include "include/common.h"
#include "metadata/metadata.h"
#include "storage/file_manager.h"

#include <algorithm>

struct SplitProperty {
  bool split = false;
  std::vector<u16> promotedKey;
  u64 rightPageId = 0;
};

BPlusTree *search(const KV &kv, u64 &pageId);
bool update(const KV &kv);
void insert(const KV &kv);
SplitProperty insertRecursive(BPlusTree &bpt, u64 pageId, const KV &kv);
