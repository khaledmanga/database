include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
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

struct PathEntry {
  u64 page;
  size_t childIndex;
};

class Pager {
private:
  std::fstream file;

public:
  explicit Pager(const std::string &filename) : file(filename, std::ios::in | std::ios::out | std::ios::binary) {

    if (!file.is_open()) {
      file.clear();

      file.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    }

    if (!file.is_open()) {
      throw std::runtime_error("Failed to open database");
    }
  }

  Page readPage(u64 pageNumber) {
    Page page{};

    std::streamoff offset = static_cast<std::streamoff>(pageNumber) * PAGE_SIZE;

    file.clear();
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

    std::streamoff size = file.tellg();

    return static_cast<u64>(size / PAGE_SIZE);
  }
};

Page encode(const BNode &node) {
  Page page{};

  size_t pageIndex = 0;

  u16 type = static_cast<u16>(node.type);

  u16 nkeys = node.nkeys;

  // type
  page[pageIndex++] = type & 0xff;
  page[pageIndex++] = (type >> 8) & 0xff;

  // nkeys
  page[pageIndex++] = nkeys & 0xff;
  page[pageIndex++] = (nkeys >> 8) & 0xff;

  // Internal node pointers
  if (node.type == BNodeType::Internal) {
    for (u64 ptr : node.pointers) {
      for (size_t j = 0; j < 8; ++j) {
        page[pageIndex++] = static_cast<u8>((ptr >> (8 * j)) & 0xff);
      }
    }
  }

  // offsets
  for (u16 offset : node.offsets) {
    page[pageIndex++] = offset & 0xff;
    page[pageIndex++] = (offset >> 8) & 0xff;
  }

  // KVs
  for (const auto &kv : node.KVs) {
    u16 keySize = static_cast<u16>(kv.key.size());
    u16 valueSize = static_cast<u16>(kv.value.size());

    // key size
    page[pageIndex++] = keySize & 0xff;
    page[pageIndex++] = (keySize >> 8) & 0xff;

    // value size
    page[pageIndex++] = valueSize & 0xff;
    page[pageIndex++] = (valueSize >> 8) & 0xff;

    // key
    for (u8 byte : kv.key) {
      page[pageIndex++] = byte;
    }

    // value
    for (u8 byte : kv.value) {
      page[pageIndex++] = byte;
    }
  }

  return page;
}

BNode decode(const Page &page) {
  BNode node;

  size_t pageIndex = 0;

  // type
  u16 type = static_cast<u16>(page[pageIndex]) | (static_cast<u16>(page[pageIndex + 1]) << 8);
  node.type = static_cast<BNodeType>(type);

  pageIndex += 2;

  // nkeys
  node.nkeys = static_cast<u16>(page[pageIndex]) | (static_cast<u16>(page[pageIndex + 1]) << 8);

  pageIndex += 2;

  // Internal node pointers
  if (node.type == BNodeType::Internal) {
    for (size_t i = 0; i < node.nkeys + 1; ++i) {

      u64 ptr = 0;

      for (size_t j = 0; j < 8; ++j) {
        ptr |= static_cast<u64>(page[pageIndex++]) << (8 * j);
      }

      node.pointers.push_back(ptr);
    }
  }

  /*
   * Your current format does not store
   * the number of offsets separately.
   *
   * Therefore there is nothing to decode
   * here unless you add offset metadata.
   */

  // KVs
  for (size_t i = 0; i < node.nkeys; ++i) {

    u16 keySize = static_cast<u16>(page[pageIndex]) | (static_cast<u16>(page[pageIndex + 1]) << 8);

    pageIndex += 2;

    u16 valueSize = static_cast<u16>(page[pageIndex]) | (static_cast<u16>(page[pageIndex + 1]) << 8);

    pageIndex += 2;

    KV kv;

    kv.key.insert(kv.key.end(), page.begin() + pageIndex, page.begin() + pageIndex + keySize);

    pageIndex += keySize;

    kv.value.insert(kv.value.end(), page.begin() + pageIndex, page.begin() + pageIndex + valueSize);

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
  size_t capacity;

private:
  BNode readNode(u64 pageNumber) {
    Page page = pager.readPage(pageNumber);

    return decode(page);
  }

  void writeNode(

    u64 pageNumber, const BNode &node) {

    Page page = encode(node);

    pager.writePage(pageNumber, page);
  }

  void createNewRoot(u64 leftPage, u64 rightPage, const std::vector<u8> &separator) {

    u64 newRootPage = pager.allocatePage();

    BNode newRoot;

    newRoot.type = BNodeType::Internal;

    newRoot.nkeys = 1;

    newRoot.KVs.push_back({separator, {}});

    newRoot.pointers.push_back(leftPage);

    newRoot.pointers.push_back(rightPage);

    writeNode(newRootPage, newRoot);

    rootPage = newRootPage;
  }

  void splitLeaf(u64 page, BNode &node, std::vector<PathEntry> &path) {

    u64 newPage = pager.allocatePage();

    BNode newNode;

    newNode.type =

        BNodeType::Leaf;

    size_t mid =

        node.KVs.size() / 2;

    // Move right half to new leaf
    newNode.KVs.insert(newNode.KVs.end(), node.KVs.begin() + mid, node.KVs.end());

    // Remove right half from old leaf
    node.KVs.erase(node.KVs.begin() + mid,

                   node.KVs.end());

    node.nkeys = static_cast<u16>(node.KVs.size());

    newNode.nkeys = static_cast<u16>(newNode.KVs.size());

    writeNode(page, node);

    writeNode(newPage,

              newNode);

    /*
     * B+Tree leaf split:
     *
     * separator = first key
     * of the RIGHT leaf.
     */
    std::vector<u8> separator = newNode.KVs.front().key;

    insertIntoParent(page, newPage, separator,

                     path);
  }

  void insertIntoParent(u64 leftPage, u64 rightPage, const std::vector<u8> &separator,

                        std::vector<PathEntry> &path) {

    /*
     * No parent means that the node
     * being split was the root.
     *
     * Therefore create a new root.
     */

    if (path.empty()) {
      createNewRoot(leftPage,

                    rightPage,

                    separator);

      return;
    }

    /*
     * Get parent information.
     */
    PathEntry parentInfo = path.back();

    path.pop_back();

    u64 parentPage = parentInfo.page;

    size_t childIndex = parentInfo.childIndex;

    BNode parent = readNode(parentPage);

    /*
     * Example:
     *
     * before:
     *
     * keys:
     * [10 30 50]
     *
     * pointers:
     * [P0 P1 P2 P3]
     *
     * childIndex = 1
     *
     * after:
     *
     * keys:
     * [10 separator 30 50]
     *
     * pointers:
     * [P0 P1 rightPage P2 P3]
     */
    parent.KVs.insert(parent.KVs.begin() + childIndex, KV{separator, {}});

    parent.pointers.insert(parent.pointers.begin() + childIndex + 1, rightPage);

    parent.nkeys++;

    /*
     * Parent still has space.

     */
    if (parent.nkeys <= capacity) {
      writeNode(parentPage, parent);

      return;
    }

    /*
     * Parent is full.
     *
     * Split internal node and
     * push its separator upward.
     */
    splitInternal(parentPage, parent, path);
  }

  void splitInternal(u64 page, BNode &node, std::vector<PathEntry> &path) {

    u64 newPage = pager.allocatePage();

    BNode newNode;

    newNode.type = BNodeType::Internal;

    /*
     * Example:
     *
     * keys:
     *
     * [10 20 30 40 50]
     *
     * mid = 2
     *
     * separator = 30

     *
     * left:
     *
     * [10 20]
     *
     * right:
     *
     * [40 50]
     *
     * 30 is pushed upward.
     */
    size_t mid = node.KVs.size() / 2;

    std::vector<u8> separator = node.KVs[mid].key;

    /*
     * Right node gets keys:
     *
     * [mid + 1, end)
     */
    newNode.KVs.insert(newNode.KVs.end(), node.KVs.begin() + mid + 1, node.KVs.end());

    /*
     * Right node gets pointers:
     *
     * [mid + 1, end)
     *
     * If there are N keys,
     * there are N + 1 pointers.
     */
    newNode.pointers.insert(newNode.pointers.end(), node.pointers.begin() + mid + 1, node.pointers.end());

    /*
     * Left node keeps:
     *
     * keys [0, mid)
     */
    node.KVs.erase(node.KVs.begin() + mid, node.KVs.end());

    /*
     * Left node keeps:
     *
     * pointers [0, mid + 1)
     */
    node.pointers.erase(node.pointers.begin() + mid + 1, node.pointers.end());

    node.nkeys = static_cast<u16>(node.KVs.size());

    newNode.nkeys = static_cast<u16>(newNode.KVs.size());

    writeNode(page, node);

    writeNode(newPage, newNode);

    /*
     * Push separator to parent.
     *
     * If parent is full,
     * insertIntoParent()
     * will call splitInternal()
     * again.
     *
     * This is the propagation.
     */
    insertIntoParent(page, newPage, separator, path);
  }

public:
  explicit BPlusTree(Pager pager, size_t capacity_)
      : rootPage(INVALID_PAGE), pager(std::move(pager)), capacity(capacity_) {}

  void insert(const std::vector<u8> &key, const std::vector<u8> &value) {

    /*
     * Empty tree.
     */
    if (rootPage == INVALID_PAGE) {
      rootPage = pager.allocatePage();

      BNode root;

      root.type = BNodeType::Leaf;

      root.nkeys = 1;

      root.KVs.push_back({key, value});

      writeNode(rootPage, root);

      return;
    }

    /*
     * Path contains every internal node
     * that we pass through.
     *
     * Example:
     *
     * root
     *   ↓
     * internal
     *   ↓
     * leaf
     *
     * path:
     *
     * [
     *   {root, childIndex},
     *   {internal, childIndex}
     * ]
     */
    std::vector<PathEntry> path;

    u64 page = rootPage;

    BNode node = readNode(page);

    /*
     * Find leaf.
     */
    while (node.type == BNodeType::Internal) {

      size_t index = 0;

      while (index < node.KVs.size() && !(key < node.KVs[index].key)) {

        ++index;
      }

      /*
       * Save parent before going
       * to the child.
       */
      path.push_back({page, index});

      page = node.pointers[index];

      node = readNode(page);
    }

    /*
     * Now:
     *
     * page = leaf page
     * node = leaf node
     */

    size_t index = 0;

    while (index < node.KVs.size() && key > node.KVs[index].key) {

      ++index;
    }

    /*
     * Existing key -> update value.
     */
    if (index < node.KVs.size() && key == node.KVs[index].key) {

      node.KVs[index].value = value;

      writeNode(page, node);

      return;
    }

    /*
     * Insert into leaf.
     */
    node.KVs.insert(node.KVs.begin() + index, KV{key, value});

    node.nkeys++;

    /*
     * Leaf has space.
     */
    if (node.nkeys <= capacity) {
      writeNode(page, node);

      return;
    }

    /*
     * Leaf overflow.
     */
    splitLeaf(page, node, path);
  }

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

      writeNode(page,

                node);

      return;
    }
  }

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

    while (index < node.KVs.size() && key > node.KVs[index].key) {

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
  try {
    Pager pager("database.db");

    BPlusTree tree(std::move(pager), 3);

    tree.insert({10}, {1});
    tree.insert({20}, {2});
    tree.insert({30}, {3});
    tree.insert({40}, {4});
    tree.insert({50}, {5});
    tree.insert({60}, {6});
    tree.insert({70}, {7});
    tree.insert({80}, {8});
    tree.insert({90}, {9});
    tree.insert({100}, {10});

    std::cout << "Insert completed\n";
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
