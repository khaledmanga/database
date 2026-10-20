#include "tree.h"

Metadata readMetadata() {
    Page page = readPage(0);
    
    Metadata metadata;
    
    metadata.decode(page);
    
    return metadata;
}

bool update(const KV& kv) {
    BPlusTree *bpt = search(kv);
    
    if(!bpt) {
        return nullptr;
    }
      
    for(int i = 0; i < bpt->KVs.size(); ++i) {
        if(bpt->KVs[i].key == kv.key) {
            bpt->KVS[i].value = kv.value;
            return true;
        }
    }
    
    return false;
}

BPlusTree *search(const KV &kv) {
    Metadata metadata = readMetadata().decode();
    Page page{};
    BPlusTree *bpt = new BPlusTree();
    
    FileManager.getInstance().readPage(metadata.root_page_id, page);
    bpt->decode(page);
    
    
    for(int i = 0; i < bpt->KVs.size(); ++i) {
        if(bpt->KVs[i].key == kv.key) {
            return bpt;
        }
    }
    
    delete bpt;
    return nullptr;
}