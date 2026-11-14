#include "bplustree.hpp"

Page convertBPlusTreeToPage(const BPlusTree &bplustree) {
  Page page(PAGE_SIZE, 0);
  std::size_t offset = 0;

  page[offset++] = static_cast<u8>(bplustree.type & 0xFFu);
  page[offset++] = static_cast<u8>((bplustree.type >> 8) & 0xFFu);
  page[offset++] = static_cast<u8>(bplustree.nkeys & 0xFFu);
  page[offset++] = static_cast<u8>((bplustree.nkeys >> 8) & 0xFFu);

  for (std::size_t i = 0; i < bplustree.pointers.size(); ++i) {
    const u64 ptr = bplustree.pointers[i];
    page[offset++] = static_cast<u8>(ptr & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 8) & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 16) & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 24) & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 32) & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 40) & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 48) & 0xFFu);
    page[offset++] = static_cast<u8>((ptr >> 56) & 0xFFu);
  }

  for (std::size_t i = 0; i < bplustree.offsets.size(); ++i) {
    const u16 offset_value = bplustree.offsets[i];
    page[offset++] = static_cast<u8>(offset_value & 0xFFu);
    page[offset++] = static_cast<u8>((offset_value >> 8) & 0xFFu);
  }

  for (std::size_t i = 0; i < bplustree.KVs.size(); ++i) {
    const auto &kv = bplustree.KVs[i];
    const u16 key_size = static_cast<u16>(kv.key.size());
    const u16 value_size = static_cast<u16>(kv.value.size());

    page[offset++] = static_cast<u8>(key_size & 0xFFu);
    page[offset++] = static_cast<u8>((key_size >> 8) & 0xFFu);
    page[offset++] = static_cast<u8>(value_size & 0xFFu);
    page[offset++] = static_cast<u8>((value_size >> 8) & 0xFFu);

    for (std::size_t j = 0; j < kv.key.size(); ++j) {
      page[offset++] = kv.key[j];
    }

    for (std::size_t j = 0; j < kv.value.size(); ++j) {
      page[offset++] = kv.value[j];
    }
  }

  return page;
}

BPlusTree convertPageToBPlusTree(const Page &page) {
  BPlusTree bpt{};
  std::size_t offset = 0;

  bpt.type =
      static_cast<u16>((static_cast<u16>(page[offset + 1]) << 8) | static_cast<u16>(page[offset]));
  offset += 2;

  bpt.nkeys =
      static_cast<u16>((static_cast<u16>(page[offset + 1]) << 8) | static_cast<u16>(page[offset]));
  offset += 2;

  const std::size_t num_pointers =
      (bpt.type == static_cast<u16>(BNodeType::LEAF)) ? 0 : bpt.nkeys + 1;

  for (std::size_t i = 0; i < num_pointers; ++i) {
    u64 ptr = 0;

    ptr |= static_cast<u64>(page[offset + 7]) << 56;
    ptr |= static_cast<u64>(page[offset + 6]) << 48;
    ptr |= static_cast<u64>(page[offset + 5]) << 40;
    ptr |= static_cast<u64>(page[offset + 4]) << 32;
    ptr |= static_cast<u64>(page[offset + 3]) << 24;
    ptr |= static_cast<u64>(page[offset + 2]) << 16;
    ptr |= static_cast<u64>(page[offset + 1]) << 8;
    ptr |= static_cast<u64>(page[offset]);

    offset += 8;
    bpt.pointers.push_back(ptr);
  }

  for (u16 i = 0; i < bpt.nkeys; ++i) {
    const u16 value = static_cast<u16>((static_cast<u16>(page[offset + 1]) << 8) |
                                       static_cast<u16>(page[offset]));
    bpt.offsets.push_back(value);
    offset += 2;
  }

  for (u16 i = 0; i < bpt.nkeys; ++i) {
    const u16 key_size = static_cast<u16>((static_cast<u16>(page[offset + 1]) << 8) |
                                          static_cast<u16>(page[offset]));
    offset += 2;
    const u16 value_size = static_cast<u16>((static_cast<u16>(page[offset + 1]) << 8) |
                                            static_cast<u16>(page[offset]));
    offset += 2;

    KV kv;

    for (u16 j = 0; j < key_size; ++j) {
      kv.key.push_back(page[offset++]);
    }

    for (u16 j = 0; j < value_size; ++j) {
      kv.value.push_back(page[offset++]);
    }

    bpt.KVs.push_back(kv);
  }

  return bpt;
}

void update_offset(BPlusTree &node) {
  node.offsets.clear();
  if (node.nkeys == 0)
    return;

  u16 current_offset = 4 + node.pointers.size() * 8 + node.nkeys * 2;
  node.offsets.push_back(current_offset);

  for (u16 i = 1; i < node.nkeys; ++i) {
    u16 prev_kv_size = 4 + node.KVs[i - 1].key.size() + node.KVs[i - 1].value.size();
    current_offset += prev_kv_size;
    node.offsets.push_back(current_offset);
  }
}
