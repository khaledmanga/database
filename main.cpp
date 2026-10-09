#include "bplustree/b_plus_tree.h"
#include "storage/file_manager.h"
#include "storage/page.h"
#include "storage/metadata.h"
#include "operators/tree.h"

int main() {
    FileManager file("database.db");

    Pager pager(file);

    Metadata metadata;

    pager.writePage(0, metadata.encode());

    BPlusTree bpt;
    bpt.typeNode = NodeType::LEAF;
    bpt.type = (u16)(NodeType::LEAF);
    bpt.nkeys = 0;

    pager.writePage(metadata.getNextPageId(), bpt.encode());

    metadata.update_metadata(pager);

    KV kv{{'a'}, {'1'}};

    metadata.update_metadata(pager);

    insert(kv, pager);

    std::optional<BPlusTree> a = search(kv, pager);

    std::cout << (char)a->KVs[0].key[0];

    file.sync();

    return 0;
}
