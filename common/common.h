#pragma once

#include <array>
#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

#define DATABASE_NUMBER 0x4441544142415345ULL

constexpr u16 PAGE_SIZE = 4096;
constexpr u8 VERSION = 1;
using Page = std::array<u8, PAGE_SIZE>;

enum class NodeType {
  LEAF,
  INTERNAL,
  ROOT,
};
