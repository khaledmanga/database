#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>

#include <optional>
#include <string>

#include <vector>

using u8 = std::uint8_t;
using u16 = std::uint16_t;

using u32 = std::uint32_t;

using u64 = std::uint64_t;

constexpr size_t PAGE_SIZE = 4096;
constexpr size_t TYPE_SIZE = 2;
constexpr size_t NKEYS_SIZE = 2;
constexpr size_t POINTER_SIZE = 8;
constexpr size_t OFFSET_SIZE = 2;
constexpr u64 INVALID_PAGE = UINT64_MAX;

using Page = std::array<u8, PAGE_SIZE>;

struct KV {
  std::vector<u8> key;
  std::vector<u8> value;
};

enum class BNodeType : u16 { Internal = 1, Leaf = 2 };

struct BNode {
  BNodeType type;
  u16 nkeys;

  std::vector<u64> pointers;
  std::vector<u16> offsets;
  std::vector<KV> KVs;
};

class Pager {
private:
  std::fstream file;

public:
  explicit Pager(const std::string &filename)
      : file(filename, std::ios::in | std::ios::out | std::ios::binary) {
    if (!file.is_open()) {
      file.clear();

      file.open(filename, std::ios::in | std::ios::out | std::ios::binary |
                              std::ios::trunc);
    }

    if (!file.is_open()) {
      throw std::runtime_error("Failed to open database");
    }
  }

  Page readPage(u64 pageNumber) {
    Page page{};

    std::streamoff offset = static_cast<std::streamoff>(pageNumber) * PAGE_SIZE;

    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char *>(page.data()), PAGE_SIZE);

    return page;
  }

  void writePage(u64 pageNumber, const Page &page) {
    std::streamoff offset = static_cast<std::streamoff>(pageNumber) * PAGE_SIZE;

    file.clear();
    file.seekp(offset, std::ios::beg);

    file.write(reinterpret_cast<const char *>(page.data()), PAGE_SIZE);

    file.flush();
  }

  u64 allocatePage() {
    file.clear();
    file.seekg(0, std::ios::end);

    const std::streamoff size = file.tellg();

    return static_cast<u64>(size / PAGE_SIZE);
  }
};

Page encode(const BNode &node) {
  Page page{};

  size_t pageIndex = 0;

  u16 type = static_cast<u16>(node.type);
  u16 nkeys = node.nkeys;

  page[pageIndex++] = type & 0xff;
  page[pageIndex++] = (type >> 8) & 0xff;

  page[pageIndex++] = nkeys & 0xff;
  page[pageIndex++] = (nkeys >> 8) & 0xff;

  if (node.type == BNodeType::Internal) {
    for (u64 ptr : node.pointers) {
      for (size_t j = 0; j < 8; ++j) {

        page[pageIndex++] = static_cast<u8>((ptr >> (8 * j)) & 0xff);
      }
    }
  }

  for (u16 offset : node.offsets) {
    page[pageIndex++] = offset & 0xff;
    page[pageIndex++] = (offset >> 8) & 0xff;
  }

  for (const auto &kv : node.KVs) {
    u16 keySize = static_cast<u16>(kv.key.size());
    u16 valueSize = static_cast<u16>(kv.value.size());

    page[pageIndex++] = keySize & 0xff;
    page[pageIndex++] = (keySize >> 8) & 0xff;

    page[pageIndex++] = valueSize & 0xff;

    page[pageIndex++] = (valueSize >> 8) & 0xff;

    for (u8 byte : kv.key) {
      page[pageIndex++] = byte;
    }

    for (u8 byte : kv.value) {

      page[pageIndex++] = byte;
    }
  }

  return page;
}

BNode decode(const Page &page) {
  BNode node;

  size_t pageIndex = 0;

  u16 type = static_cast<u16>(page[pageIndex]) |
             (static_cast<u16>(page[pageIndex + 1]) << 8);

  node.type = static_cast<BNodeType>(type);
  pageIndex += 2;

  node.nkeys = static_cast<u16>(page[pageIndex]) |
               (static_cast<u16>(page[pageIndex + 1]) << 8);

  pageIndex += 2;

  if (node.type == BNodeType::Internal) {
    for (size_t i = 0; i < node.nkeys + 1; ++i) {
      u64 ptr = 0;

      for (size_t j = 0; j < 8; ++j) {
        ptr |= static_cast<u64>(page[pageIndex++]) << (8 * j);
      }

      node.pointers.push_back(ptr);
    }
  }

  for (size_t i = 0; i < node.offsets.size(); ++i) {
    u16 offset = static_cast<u16>(page[pageIndex]) |
                 (static_cast<u16>(page[pageIndex + 1]) << 8);

    pageIndex += 2;

    node.offsets.push_back(offset);
  }

  for (size_t i = 0; i < node.nkeys; ++i) {

    u16 keySize = static_cast<u16>(page[pageIndex]) |
                  (static_cast<u16>(page[pageIndex + 1]) << 8);

    pageIndex += 2;

    u16 valueSize = static_cast<u16>(page[pageIndex]) |
                    (static_cast<u16>(page[pageIndex + 1]) << 8);

    pageIndex += 2;

    KV kv;

    kv.key.insert(kv.key.end(), page.begin() + pageIndex,
                  page.begin() + pageIndex + keySize);

    pageIndex += keySize;

    kv.value.insert(kv.value.end(),

                    page.begin() + pageIndex,

                    page.begin() + pageIndex + valueSize);

    pageIndex += valueSize;

    node.KVs.push_back(std::move(kv));
  }

  return node;
}

size_t nodeSize(const BNode &node) {
  size_t size = TYPE_SIZE + NKEYS_SIZE;

  size += node.pointers.size() * POINTER_SIZE;
  size += node.offsets.size() * OFFSET_SIZE;

  for (const auto &kv : node.KVs) {
    size += 2;
    size += 2;
    size += kv.key.size();
    size += kv.value.size();
  }

  return size;
}

class BPlusTree {
private:
  u64 rootPage;
  Pager pager;
  int capacity;

  BNode readNode(u64 pageNumber) {
    Page page = pager.readPage(pageNumber);
    return decode(page);
  }

  void writeNode(u64 pageNumber, const BNode &node) {
    Page page = encode(node);
    pager.writePage(pageNumber, page);
  }

public:
  explicit BPlusTree(Pager pager, int capacity_)
      : pager(std::move(pager)), rootPage(INVALID_PAGE), capacity(capacity_) {}

  // TODO: Balance tree after inserting
  void insert(const std::vector<u8> &key, const std::vector<u8> &value) {
    u64 page;
    BNode node;

    if (rootPage == INVALID_PAGE) {
      rootPage = pager.allocatePage();

      page = rootPage;

      node.type = BNodeType::Leaf;
      node.nkeys = 0;
    } else {
      page = rootPage;
      node = readNode(page);
    }

    while (node.type == BNodeType::Internal) {
      size_t index = 0;

      while (index < node.KVs.size() && !(key < node.KVs[index].key)) {
        ++index;
      }

      page = node.pointers[index];

      node = readNode(page);
    }

    BNode parent = node;

    size_t index = 0;

    while (index < node.KVs.size() && key < node.KVs[index].key) {
      ++index;
    }

    // When key is existed but doesn't have value
    if (index < node.KVs.size() && key == node.KVs[index].key) {
      node.KVs[index].value = value;

      writeNode(page, node);

      return;
    }

    node.KVs.insert(node.KVs.begin() + index, KV{key, value});

    node.nkeys++;
    
    if(nkeys < this->capacity) {
      writeNode(page, node);
    } 

    // Case 1: if only leaf and leaf is full
    
    newPage = this->pager.allocatePage();
    BNode newNode;
    newNode.type = BNodeType::Leaf;
    
    int mid = nkeys / 2;

    BNode.KVs.insert(
      newNode.begin(),
      node.KVs.begin() + mid,
      node.KVs.end()
    )

    node.KVs.erase(
      node.KVs.begin() + mid,
      node.KVs.end()
    )

    node.nkeys -= mid;
    newNode.nkeys = nkeys - mid;

    writeNode(page, node);
    writeNode(newPage, newNode);

    KV separator = newNode.KVs[0];

    BNode newRoot;
    newRoot.type = BNodeType::Internal;
    newRoot.nkeys = 1;
    newRoot.KVs.push_back(separator);
    newRoot.pointers.push_back(page);
    newRoot.pointers.push_back(newPage);
  }

  // TODO: Balance tree after updating
  void update(const std::vector<u8> &key, const std::vector<u8> &value) {
    if (rootPage == INVALID_PAGE) {
      return;
    }

    u64 page = rootPage;
    BNode node = readNode(page);

    while (node.type == BNodeType::Internal) {
      size_t index = 0;

      while (index < node.KVs.size() && !(key < node.KVs[index].key)) {
        ++index;
      }

      page = node.pointers[index];
      node = readNode(page);
    }

    size_t index = 0;

    while (index < node.KVs.size() && key < node.KVs[index].key) {
      ++index;
    }

    if (index < node.KVs.size() && key == node.KVs[index].key) {
      node.KVs[index].value = value;

      writeNode(page, node);

      return;
    }
  }

  /**
   * TODO:
   * 1. Balance tree after removing
   * 2. Handle empty node after removing
   */
  void remove(const std::vector<u8> &key) {
    if (rootPage == INVALID_PAGE) {
      return;
    }

    u64 page = rootPage;
    BNode node = readNode(page);

    while (node.type == BNodeType::Internal) {
      size_t index = 0;

      while (index < node.KVs.size() && !(key < node.KVs[index].key)) {
        ++index;
      }

      page = node.pointers[index];
      node = readNode(page);
    }

    size_t index = 0;

    while (index < node.KVs.size() && key < node.KVs[index].key) {
      ++index;
    }

    if (index < node.KVs.size() && key == node.KVs[index].key) {
      node.KVs.erase(node.KVs.begin() + index);

      node.nkeys--;

      writeNode(page, node);

      return;
    }
  }

  std::optional<BNode> search(const std::vector<u8> &key) {
    if (rootPage == INVALID_PAGE) {
      return std::nullopt;
    }

    BNode node = readNode(rootPage);

    while (node.type == BNodeType::Internal) {
      size_t index = 0;

      while (index < node.KVs.size() && !(key < node.KVs[index].key)) {
        ++index;
      }

      u64 childPage = node.pointers[index];

      node = readNode(childPage);
    }

    return node;
  }
};

int main() {
  return 0;
}
