#include "tree.h"

Metadata readMetadata() {
    Page page = readPage(0);
    
    Metadata metadata;
    
    metadata.decode(page);
    
    return metadata;
}

*BPlusTree search(const KV &kv) {
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