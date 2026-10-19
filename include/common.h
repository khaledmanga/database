#pragma once

#include <vector>
#include <stdint.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

const int PAGE_SIZE = 4096;
using Page = std::vector<u8>;

enum class BNodeType {
  ROOT,
  INTERNAL,
  LEAF,
};