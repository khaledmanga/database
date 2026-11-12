#include "bplustree.hpp"

Page convertBPlusTreeToPage(const BPlusTree& bplustree) {
    Page page(PAGE_SIZE);

    unsigned int offset = 0;
    page[offset++] = bplustree.type & 0xFF;
    page[offset++] = bplustree.type >> 8 & 0xFF;
    page[offset++] = bplustree.nkeys & 0xFF;
    page[offset++] = bplustree.nkeys >> 8 & 0xFF;

    for(size_t i = 0; i < this.pointers.size(); ++i) {
        page[offset++] = this.pointers[i] & 0xFF;
        page[offset++] = this.pointers[i] >> 8 & 0xFF;
        page[offset++] = this.pointers[i] >> 16 & 0xFF;
        page[offset++] = this.pointers[i] >> 24 & 0xFF;
        page[offset++] = this.pointers[i] >> 32 & 0xFF;
        page[offset++] = this.pointers[i] >> 40 & 0xFF;
        page[offset++] = this.pointers[i] >> 48 & 0xFF;
        page[offset++] = this.pointers[i] >> 56 & 0xFF;
    }

    for(size_t i = 0; i < this.offsets.size(); ++i) {
        page[offset++] = this.offsets[i] & 0xFF;
        page[offset++] = this.offsets[i] >> 8 & 0xFF;
    }

    for(size_t i = 0; i < this.KVs.size(); ++i) {
        u16 key_size = this.KVs[i].key.size();
        u16 value_size = this.KVs[i].value.size();

        page[offset++] = key_size & 0xFF;
        page[offset++] = key_size >> 8 & 0xFF;
        page[offset++] = value_size & 0xFF;
        page[offset++] = value_size >> 8 & 0xFF;

        for(size_t j = 0; j < this.KVs[i].key.size(); ++j) {
            page[offset++] = this.KVs[i].key[j];
        }

        for(size_t j = 0; j < this.KVs[i].value.size(); ++j) {
            page[offset++] = this.KVs[i].value[j];
        }
    }
}

BPlusTree convertPageToBPlusTree(const Page& page) {
    
}