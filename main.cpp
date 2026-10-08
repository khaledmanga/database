#include "bplustree/b_plus_tree.h"
#include "storage/file_manager.h"
#include "storage/page.h"
#include "storage/metadata.h"

int main() {
    FileManager file("database.db");
    Pager pager(file);
    Metadata metadata;

    Page page = metadata.encode();

    pager.writePage(metadata.getRootPageId(), page);

    file.sync();

    return 0;
}