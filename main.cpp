#include "bplustree/b_plus_tree.h"
#include "storage/page.h"
#include "storage/file_manager.h"

int main() {
	FileManager::open("database.db");

	BPlusTree tree;

	Page page = tree.encode();

	FileManager::write(0, page.data.data(), PAGE_SIZE);

	FileManager::close();

	return 0;
}
