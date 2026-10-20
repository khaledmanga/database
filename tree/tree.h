#pragma once

#include "include/common.h"

class Metadata {
    public:
        u64 root_page_id;
        u64 next_page_id;
        
        Page encode();
        void decode(const Page& page);
};

bool insert(const KV& kv);
bool update(const KV& kv);
BPlusTree *search(const KV &kv);
Metadata readMetadata();