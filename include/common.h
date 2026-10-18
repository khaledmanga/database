#pragma once

#define uint8 u8
#define uint16 u16
#define uint32 u32
#define uint64 u64

const PAGE_SIZE;
using Page = std::vector<u8, PAGE_SIZE>;

enum class BNodeType {
  ROOT,
  INTERNAL,
  LEAF,
};