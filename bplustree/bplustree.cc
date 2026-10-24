#include "bplustree.h"

Page encodeBPlusTree(const BPlusTree &bpt) {
  Page page(PAGE_SIZE, 0);

  int offset = 0;
  page[offset++] = (u8)(bpt.types & 0xFF);
  page[offset++] = (u8)(bpt.types >> 8 & 0xFF);

  page[offset++] = (u8)(bpt.nkeys & 0xFF);
  page[offset++] = (u8)(bpt.nkeys >> 8 & 0xFF);

  for (size_t i = 0; i < bpt.pointers.size(); ++i) {
    page[offset++] = (u8)(bpt.pointers[i] & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 8 & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 16 & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 24 & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 32 & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 40 & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 48 & 0xFF);
    page[offset++] = (u8)(bpt.pointers[i] >> 56 & 0xFF);
  }

  for (size_t i = 0; i < bpt.offset.size(); ++i) {
    page[offset++] = (u8)(bpt.offset[i] & 0xFF);
    page[offset++] = (u8)(bpt.offset[i] >> 8 & 0xFF);
  }

  for (size_t i = 0; i < bpt.KVs.size(); ++i) {
    page[offset++] = (u8)(bpt.KVs[i].key.size() & 0xFF);
    page[offset++] = (u8)(bpt.KVs[i].key.size() >> 8 & 0xFF);
    page[offset++] = (u8)(bpt.KVs[i].value.size() & 0xFF);
    page[offset++] = (u8)(bpt.KVs[i].value.size() >> 8 & 0xFF);

    for (size_t j = 0; j < bpt.KVs[i].key.size(); ++j) {
      page[offset++] = (u8)(bpt.KVs[i].key[j] & 0xFF);
      page[offset++] = (u8)(bpt.KVs[i].key[j] >> 8 & 0xFF);
    }

    for (size_t j = 0; j < bpt.KVs[i].value.size(); ++j) {
      page[offset++] = (u8)(bpt.KVs[i].value[j] & 0xFF);
      page[offset++] = (u8)(bpt.KVs[i].value[j] >> 8 & 0xFF);
    }
  }

  return page;
}

BPlusTree decodeBPlusTree(const Page &page) {
  int offset = 0;
  BPlusTree bpt;

  bpt.types = ((u16)(page[offset + 1]) << 8) | (u16)page[offset];
  offset += 2;

  bpt.nkeys = ((u16)(page[offset + 1]) << 8) | (u16)page[offset];
  offset += 2;

  size_t ptr_nums = 0;
  if ((BNodeType)bpt.types != BNodeType::LEAF) {
    ptr_nums = bpt.nkeys + 1;
  }

  for (size_t i = 0; i < ptr_nums; ++i) {
    u64 ptr = 0;

    ptr |= (u64)page[offset];
    ptr |= (u64)page[offset + 1] << 8;
    ptr |= (u64)page[offset + 2] << 16;
    ptr |= (u64)page[offset + 3] << 24;
    ptr |= (u64)page[offset + 4] << 32;
    ptr |= (u64)page[offset + 5] << 40;
    ptr |= (u64)page[offset + 6] << 48;
    ptr |= (u64)page[offset + 7] << 56;
    bpt.pointers.push_back(ptr);

    offset += 8;
  }

  for (u16 i = 0; i < bpt.nkeys; ++i) {
    u16 value = 0;

    value |= (u16)page[offset + 1] << 8;
    value |= (u16)page[offset];

    bpt.offset.push_back(value);
    offset += 2;
  }

  for (size_t i = 0; i < bpt.nkeys; ++i) {
    u16 key_size = (u16)page[offset + 1] << 8 | (u16)page[offset];
    offset += 2;

    u16 value_size = (u16)page[offset + 1] << 8 | (u16)page[offset];
    offset += 2;

    KV kv;

    for (size_t j = 0; j < key_size; ++j) {
      kv.key.push_back((u16)page[offset + 1] << 8 | (u16)page[offset]);
      offset += 2;
    }

    for (size_t j = 0; j < value_size; ++j) {
      kv.value.push_back((u16)page[offset + 1] << 8 | (u16)page[offset]);
      offset += 2;
    }

    bpt.KVs.push_back(kv);
  }

  return bpt;
}

void BPlusTree::update_offset() {
  nkeys = (u16)(KVs.size());
  offset.clear();

  if (nkeys == 0) {
    return;
  }

  offset.resize(nkeys);
  offset[0] = 4 + pointers.size() * 8 + nkeys * 2;

  for (int i = 1; i < nkeys; ++i) {
    offset[i] = offset[i - 1] + 4 + KVs[i - 1].key.size() * 2 + KVs[i - 1].value.size() * 2;
  }
}
