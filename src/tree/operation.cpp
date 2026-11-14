#include "operation.hpp"

RecursiveInsert splitLeaf(u8 page_id, struct BPlusTree &bpt) {
  size_t mid = bpt.nkeys / 2;

  std::vector<u8> separator_key = bpt.KVs[mid].key;
  BPlusTree right_bpt;

  right_bpt.KVs.insert(right_bpt.KVs.end(), bpt.KVs.begin() + mid, bpt.KVs.end());
  right_bpt.type = bpt.type;
  right_bpt.nkeys = static_cast<u16>(right_bpt.KVs.size());

  bpt.KVs.resize(mid);
  bpt.nkeys = static_cast<u16>(bpt.KVs.size());

  update_offset(right_bpt);
  update_offset(bpt);

  u8 allocated_page_id = allocatePage();

  writePage(page_id, convertBPlusTreeToPage(bpt));
  writePage(allocated_page_id, convertBPlusTreeToPage(right_bpt));

  return {allocated_page_id, separator_key};
}

RecursiveInsert splitInternal(u8 page_id, struct BPlusTree &bpt) {
  size_t mid = bpt.nkeys / 2;

  std::vector<u8> separator_key = bpt.KVs[mid].key;

  BPlusTree right_bpt;

  right_bpt.type = bpt.type;
  right_bpt.KVs.insert(right_bpt.KVs.end(), bpt.KVs.begin() + mid, bpt.KVs.end());
  right_bpt.nkeys = static_cast<u16>(right_bpt.KVs.size());

  bpt.KVs.resize(mid);
  bpt.nkeys = static_cast<u16>(bpt.KVs.size());
  u8 allocated_page_id = allocatePage();

  update_offset(bpt);
  update_offset(right_bpt);

  writePage(page_id, convertBPlusTreeToPage(bpt));
  writePage(allocated_page_id, convertBPlusTreeToPage(right_bpt));

  return {allocated_page_id, separator_key};
}

void insert(u8& root_page_id, const KV &kv) {
  BPlusTree bpt = convertPageToBPlusTree(readPage(root_page_id));
  RecursiveInsert ri = insertRecursive(root_page_id, kv);

  if (ri.right_page_id == 0) {
    return;
  }

  BPlusTree new_root;

  new_root.type = static_cast<u8>(BNodeType::ROOT);
  new_root.nkeys = 1;
  new_root.KVs.push_back(KV{ri.separator_key});
  new_root.pointers.push_back(root_page_id);
  new_root.pointers.push_back(ri.right_page_id);

  update_offset(new_root);

  u8 new_root_page_id = allocatePage();

  writePage(new_root_page_id, convertBPlusTreeToPage(new_root));

  root_page_id = new_root_page_id;
}

RecursiveInsert insertRecursive(u8 page_id, const KV &kv) {
  BPlusTree bpt = convertPageToBPlusTree(readPage(page_id));

  if (static_cast<BNodeType>(bpt.type) == BNodeType::LEAF) {
    u16 i = 0;

    while (i < bpt.nkeys && bpt.KVs[i].key < kv.key) {
      ++i;
    }

    bpt.KVs.insert(bpt.KVs.begin() + i, kv);
    bpt.nkeys = static_cast<u16>(bpt.KVs.size());
    update_offset(bpt);

    if (bpt.nkeys <= MAX_KEYS) {
      writePage(page_id, convertBPlusTreeToPage(bpt));

      return {};
    }

    return splitLeaf(page_id, bpt);

  } else if (static_cast<BNodeType>(bpt.type) != BNodeType::LEAF) {
    u16 i = 0;

    while (i < bpt.nkeys && bpt.KVs[i].key < kv.key) {
      ++i;
    }

    RecursiveInsert ri = insertRecursive(bpt.pointers[i], kv);

    if (ri.right_page_id == 0) {
      return {};
    }

    bpt.KVs.insert(bpt.KVs.begin() + i, KV{ri.separator_key});
    bpt.pointers.insert(bpt.pointers.begin() + i + 1, ri.right_page_id);
    bpt.nkeys = static_cast<u16>(bpt.KVs.size());

    update_offset(bpt);

    if (bpt.nkeys <= MAX_KEYS) {
      writePage(page_id, convertBPlusTreeToPage(bpt));

      return {};
    }
  }

  return {};
}