#include "b_plus_tree.h"

Page BPlusTree::encode() {
  Page page;

    int offset = 0;

    page[offset++] = this->type & 0xFF;
    page[offset++] = (this->type >> 8) & 0xFF;

    for (size_t i = 0; i < this->pointers.size(); ++i) {
      u64 pointer = pointers[i];

      for(int i = 0; i < 8; ++i) {
        page[offset++] = (u8)(pointer >> i & 0xFF);
      }
    }

    for(size_t i = 0; i < this->offsets.size(); ++i) {
      u16 offset = offsets[i];

      for(int i = 0; i < 2; i++) {
        page[offset++] = (u8)(offset >> i & 0xFF);
      }
    }

    for(size_t i = 0; i < this->KVs.size(); ++i) {
      KV kv = this->KVs[i];

      u16 key_size = kv.key;
      u16 value_size = kv.value;

      for(int i = 0; i < 2; ++i) {
        page[offset++] = (u8)(key_size >> i & 0xFF);
      }

      for(int i = 0; i < 2; ++i) {
        page[offset++] = (u8)(value_size >> i & 0xFF);
      }

      for(int i = 0; i < 2; ++i) {
        page[offset++] = (u8)(kv.key >> i & 0xFF);        
      }

      for(int i = 0; i < 2; ++i) {
        page[offset++] = (u8)(kv.value >> i & 0xFF);        
      }
    }

  return page;
}

BPlusTree BPlusTree::decode() {
  BPlusTree bpt;
    
    
  return bpt;
}
