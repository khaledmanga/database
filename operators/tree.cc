#include "tree.h"

u64 find_leaf(const std::vector<u16> &key, Pager &pager, u64 page_id) {
  Page page = pager.readPage(page_id);

  BPlusTree bpt;
  bpt = bpt.decode(page);

  if (bpt.typeNode != NodeType::ROOT && bpt.typeNode != NodeType::INTERNAL) {
    return page_id;
  }

  std::size_t i = 0;

  while (i < bpt.KVs.size() && key >= bpt.KVs[i].key) {
    ++i;
  }

  return find_leaf(key, pager, bpt.pointers[i]);
}

void insert(const KV &kv, Pager &pager) {
  Page page = pager.readPage(0);

  Metadata metadata;
  metadata = metadata.decode(page);

  u64 root_page_id = metadata.getRootPageId();

  u64 leaf_page_id = find_leaf(kv.key, pager, root_page_id);

  Page leaf_page = pager.readPage(leaf_page_id);

  BPlusTree leaf;
  leaf = leaf.decode(leaf_page);

  leaf.KVs.insert(leaf.KVs.end(), kv);
  ++leaf.nkeys;
  leaf.update_offset();

  pager.writePage(leaf_page_id, leaf.encode());
}

std::optional<BPlusTree> search(const KV &kv, Pager &pager) {
  Page page = pager.readPage(0);

  Metadata metadata;
  metadata = metadata.decode(page);

  u64 root_page_id = metadata.getRootPageId();
  u64 leaf_page_id = find_leaf(kv.key, pager, root_page_id);

  Page leaf_page = pager.readPage(leaf_page_id);

  BPlusTree leaf;
  leaf = leaf.decode(leaf_page);

  for (const KV &kv_ : leaf.KVs) {
    if (kv_.key == kv.key && kv_.value == kv.value) {
      return leaf;
    }
  }

  return std::nullopt;
}