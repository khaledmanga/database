#include "b_plus_tree.h"

Page BPlusTree::encode() {
  Page page{};
  int offset = 0;

  page[offset++] = (u8)(this->type & 0xFF);
  page[offset++] = (u8)((this->type >> 8) & 0xFF);

  page[offset++] = (u8)(this->nkeys & 0xFF);
  page[offset++] = (u8)((this->nkeys >> 8) & 0xFF);

  for (u64 pointer : this->pointers) {
    for (int i = 0; i < 8; ++i) {
      page[offset++] = (u8)((pointer >> (i * 8)) & 0xFF);
    }
  }

  for (u16 off : this->offsets) {
    page[offset++] = (u8)(off & 0xFF);
    page[offset++] = (u8)((off >> 8) & 0xFF);
  }

  for (const KV &kv : this->KVs) {
    u16 key_size = (u16)kv.key.size();
    u16 value_size = (u16)kv.value.size();

    page[offset++] = (u8)(key_size & 0xFF);
    page[offset++] = (u8)((key_size >> 8) & 0xFF);

    page[offset++] = (u8)(value_size & 0xFF);
    page[offset++] = (u8)((value_size >> 8) & 0xFF);

    for (u16 ch : kv.key) {
      page[offset++] = (u8)(ch & 0xFF);
      page[offset++] = (u8)((ch >> 8) & 0xFF);
    }

    for (u16 ch : kv.value) {
      page[offset++] = (u8)(ch & 0xFF);
      page[offset++] = (u8)((ch >> 8) & 0xFF);
    }
  }

  return page;
}

BPlusTree BPlusTree::decode(const Page &page) {
  BPlusTree bpt;
  int offset = 0;

  bpt.type = (u16)page[offset] | (u16)page[offset + 1] << 8;
  offset += 2;

  bpt.nkeys = (u16)page[offset] | (u16)page[offset + 1] << 8;
  offset += 2;
  bpt.typeNode = (NodeType)(bpt.type);

  const u16 pointer_count = bpt.typeNode == NodeType::ROOT || bpt.typeNode == NodeType::INTERNAL ? bpt.nkeys + 1 : 0;

  for (u16 i = 0; i < pointer_count; ++i) {
    u64 pointer = 0;

    for (int j = 0; j < 8; ++j) {
      pointer |= (u64)page[offset + j] << (j * 8);
    }

    bpt.pointers.push_back(pointer);
    offset += 8;
  }

  for (u16 i = 0; i < bpt.nkeys; ++i) {
    u16 off = (u16)page[offset] | (u16)page[offset + 1] << 8;

    bpt.offsets.push_back(off);
    offset += 2;
  }

  for (u16 i = 0; i < bpt.nkeys; ++i) {
    u16 key_size = (u16)page[offset] | (u16)page[offset + 1] << 8;
    offset += 2;

    u16 value_size = (u16)page[offset] | (u16)page[offset + 1] << 8;
    offset += 2;

    KV kv;

    for (u16 j = 0; j < key_size; ++j) {
      u16 ch = (u16)page[offset] | (u16)page[offset + 1] << 8;

      kv.key.push_back(ch);
      offset += 2;
    }

    for (u16 j = 0; j < value_size; ++j) {
      u16 ch = (u16)page[offset] | (u16)page[offset + 1] << 8;

      kv.value.push_back(ch);
      offset += 2;
    }

    bpt.KVs.push_back(kv);
  }

  return bpt;
}

void BPlusTree::update_offset() {
  offsets.clear();

  u16 offset = (u16)(4 + pointers.size() * 8 + KVs.size() * 2);

  for (const KV &kv : KVs) {
    offsets.push_back(offset);

    offset += (u16)(4 + kv.key.size() * 2 + kv.value.size() * 2);
  }
}
