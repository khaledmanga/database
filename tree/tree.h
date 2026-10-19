#pragma once

class Metadata {
    public:
        u64 root_page_id;
        u64 next_page_id;
        
        Page encode();
        void decode(const Page& page);
};

void insert(const BPlusTree &blt);
void update(const BPlusTree &blt);
BPlusTree search();
Metadata readMetadata();