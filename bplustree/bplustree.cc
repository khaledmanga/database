#include "bplustree.h"

Page BPlusTree::encode() {
  Page page(PAGE_SIZE, 0);

  int offset = 0;
  page[offset++] = (u8)(this->types & 0xFF);
  page[offset++] = (u8)(this->types >> 8 & 0xFF);

  page[offset++] = (u8)(this->nkeys & 0xFF);
  page[offset++] = (u8)(this->nkeys >> 8 & 0xFF);

  for (size_t i = 0; this->pointers.size(); ++i) {
    page[offset++] = (u8)(this->pointers[i] & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 8 & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 16 & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 24 & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 32 & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 40 & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 48 & 0xFF);
    page[offset++] = (u8)(this->pointers[i] >> 56 & 0xFF);
  }

  for (size_t i = 0; this->offset.size(); ++i) {
    page[offset++] = (u8)(this->offset[i] & 0xFF);
    page[offset++] = (u8)(this->offset[i] >> 8 & 0xFF);
  }

  for (size_t i = 0; i < this->KVs.size(); ++i) {
    page[offset++] = (u8)(this->KVs[i].key.size() & 0xFF);
    page[offset++] = (u8)(this->KVs[i].key.size() >> 8 & 0xFF);
    page[offset++] = (u8)(this->KVs[i].value.size() & 0xFF);
    page[offset++] = (u8)(this->KVs[i].value.size() >> 8 & 0xFF);

    for (size_t i = 0; i < this->KVs[i].key.size(); ++i) {
      page[offset++] = (u8)(this->KVs[i].key[i] & 0xFF);
      page[offset++] = (u8)(this->KVs[i].key[i] >> 8 & 0xFF);
    }

    for (size_t i = 0; i < this->KVs[i].value.size(); ++i) {
      page[offset++] = (u8)(this->KVs[i].value[i] & 0xFF);
      page[offset++] = (u8)(this->KVs[i].value[i] >> 8 & 0xFF);
    }
  }

  return page;
}

void BPlusTree::decode(const Page &page) {
  int offset = 0;

  this->types = ((u16)(page[offset + 1]) << 8) | (u16)page[offset];
  offset += 2;

  this->nkeys = ((u16)(page[offset + 1]) << 8) | (u16)page[offset];
  offset += 2;

  size_t ptr_nums = 0;
  if ((BNodeType)this->types != BNodeType::LEAF) {
    ptr_nums = this->nkeys + 1;
  }

  for (size_t i = 0; i < ptr_nums; ++i) {
    u64 ptr = 0;

    ptr |= (u64)page[offset] << 56;
    ptr |= (u64)page[offset + 1] << 48;
    ptr |= (u64)page[offset + 2] << 40;
    ptr |= (u64)page[offset + 3] << 32;
    ptr |= (u64)page[offset + 4] << 24;
    ptr |= (u64)page[offset + 5] << 16;
    ptr |= (u64)page[offset + 6] << 8;
    ptr |= (u64)page[offset + 7];
    this->pointers.push_back(ptr);

    offset += 8;
  }

  for (u16 i = 0; i < this->nkeys; ++i) {
    u16 offset = 0;

    offset |= (u16)page[offset + 1] << 8;
    offset |= (u16)page[offset];

    this->offset.push_back(offset);
    offset += 2;
  }

  for (size_t i = 0; i < this->KVs.size(); ++i) {
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

    this->KVs.push_back(kv);
  }
}

void BPlusTree::update_offset() {
  this->nkeys = (u16)(this->KVs.size());
  this->offset.clear();

  if (this->nkeys == 0) {
    return;
  }

  this->offset.resize(this->nkeys);
  this->offset[0] = 4 + this->pointers.size() * 8 + this->nkeys * 4;

  for (int i = 1; i < this->nkeys; ++i) {
    this->offset[i] = this->offset[i - 1] + 4 + this->KVs[i - 1].key.size() + this->KVs[i - 1].value.size();
  }
}
