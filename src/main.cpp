#include "storage/metadata.hpp"
#include "storage/page.hpp"
#include "tree/operation.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: database_cli <key> <value>\n";
    return 1;
  }

  Metadata metadata = convertPageToMetadata(readPage(0));
  if (metadata.root_page_id == 0 && metadata.next_page_id == 0) {
    metadata = {1, 2};

    BPlusTree root;
    root.type = static_cast<u16>(BNodeType::LEAF);
    root.nkeys = 0;

    if (!writePage(0, convertMetadataToPage(metadata)) ||
        !writePage(metadata.root_page_id, convertBPlusTreeToPage(root))) {
      std::cerr << "Failed to initialize database\n";
      return 1;
    }
  } else if (metadata.root_page_id == 0 || metadata.next_page_id == 0) {
    std::cerr << "Database metadata is invalid\n";
    return 1;
  }

  const std::string key(argv[1]);
  const std::string value(argv[2]);
  KV kv{
      std::vector<u8>(key.begin(), key.end()),
      std::vector<u8>(value.begin(), value.end()),
  };

  insert(metadata.root_page_id, kv);
  if (!writePage(0, convertMetadataToPage(metadata))) {
    std::cerr << "Failed to write database metadata\n";
    return 1;
  }

  return 0;
}
