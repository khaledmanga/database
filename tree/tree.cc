
#include "tree.h"

BPlusTree *search(const KV &kv, u64 &pageId) {
  Metadata metadata = FileManager::getInstance().readMetadata();
  pageId = metadata.root_page_id;

  while (true) {
    Page page = FileManager::getInstance().readPage(pageId);
    BPlusTree *bpt = new BPlusTree(decodeBPlusTree(page));

    if ((BNodeType)bpt->types == BNodeType::LEAF) {
      for (const KV &item : bpt->KVs) {
        if (item.key == kv.key) {
          return bpt;
        }
      }

      delete bpt;
      return nullptr;
    }

    size_t i = 0;

    while (i < bpt->KVs.size() && !(kv.key < bpt->KVs[i].key)) {
      ++i;
    }

    if (i >= bpt->pointers.size()) {
      delete bpt;
      return nullptr;
    }

    pageId = bpt->pointers[i];
    delete bpt;
  }
}

bool update(const KV &kv) {
  u64 pageId = 0;
  BPlusTree *bpt = search(kv, pageId);

  if (!bpt) {
    return false;
  }

  for (KV &item : bpt->KVs) {
    if (item.key == kv.key) {
      item.value = kv.value;
      bpt->update_offset();

      bool success = FileManager::getInstance().writePage(pageId, encodeBPlusTree(*bpt));

      delete bpt;
      return success;
    }
  }

  delete bpt;
  return false;
}

void insert(const KV &kv) {
  Metadata metadata = FileManager::getInstance().readMetadata();
  u64 rootId = metadata.root_page_id;
  Page page = FileManager::getInstance().readPage(rootId);
  BPlusTree rootNode = decodeBPlusTree(page);

  SplitProperty sp = insertRecursive(rootNode, rootId, kv);

  if (sp.split) {
  BPlusTree newRoot;
  newRoot.types = (u16)BNodeType::INTERNAL;
  newRoot.KVs = {KV{sp.promotedKey, {}}};
  newRoot.pointers = {rootId, sp.rightPageId};
  newRoot.nkeys = 1;
  newRoot.update_offset();

  metadata = FileManager::getInstance().readMetadata();  
  u64 new_page_id = metadata.next_page_id;

  FileManager::getInstance().writePage(new_page_id, encodeBPlusTree(newRoot));

  metadata.root_page_id = new_page_id;   
  metadata.next_page_id += 1;         
  FileManager::getInstance().writePage(0, encodeMetadata(metadata));
}
}

SplitProperty insertRecursive(BPlusTree &bpt, u64 pageId, const KV &kv) {
  if ((BNodeType)bpt.types == BNodeType::LEAF) {
    size_t i = 0;
    while (i < bpt.nkeys && bpt.KVs[i].key <= kv.key) {
      ++i;
    }

    bpt.KVs.insert(bpt.KVs.begin() + i, kv);
    bpt.nkeys = bpt.KVs.size();
    bpt.update_offset();
  } else if ((BNodeType)bpt.types == BNodeType::INTERNAL) {
    size_t i = 0;
    while (i < bpt.KVs.size() && bpt.KVs[i].key <= kv.key) {
      ++i;
    }

    Page page = FileManager::getInstance().readPage(bpt.pointers[i]);
    BPlusTree node = decodeBPlusTree(page);

    SplitProperty sp = insertRecursive(node, bpt.pointers[i], kv);

    if (!sp.split) {
      return {};
    }

    bpt.KVs.insert(bpt.KVs.begin() + i, KV{sp.promotedKey, {}});
    bpt.pointers.insert(bpt.pointers.begin() + i + 1, sp.rightPageId);
    bpt.nkeys = bpt.KVs.size();
    bpt.update_offset();
  }

  if (bpt.KVs.size() <= MAXIMUM_CHILDREN_IN_ONE_NODE) {
    FileManager::getInstance().writePage(pageId, encodeBPlusTree(bpt));
    return {};
  }

  size_t mid = bpt.KVs.size() / 2;

  std::vector<u16> promotedKey = bpt.KVs[mid].key;

  BPlusTree rightNode;

  rightNode.types = bpt.types;

  Metadata metadata = FileManager::getInstance().readMetadata();
  u64 right_page_id = metadata.next_page_id;

  if ((BNodeType)bpt.types == BNodeType::LEAF) {
    rightNode.KVs.assign(bpt.KVs.begin() + mid, bpt.KVs.end());
    bpt.KVs.resize(mid);
  } else {
    rightNode.KVs.assign(bpt.KVs.begin() + mid + 1, bpt.KVs.end());
    rightNode.pointers.assign(bpt.pointers.begin() + mid + 1, bpt.pointers.end());
    bpt.KVs.resize(mid);
    bpt.pointers.resize(mid + 1);
  }

  bpt.nkeys = bpt.KVs.size();
  bpt.update_offset();
  rightNode.nkeys = rightNode.KVs.size();
  rightNode.update_offset();

  FileManager::getInstance().writePage(pageId, encodeBPlusTree(bpt));
  FileManager::getInstance().writePage(right_page_id, encodeBPlusTree(rightNode));
  metadata.updateNextPageId();

  return {true, promotedKey, right_page_id};
}
