#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include <cstdint>
#include <vector>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using Page = std::vector<u8>;

inline constexpr std::size_t PAGE_SIZE = 4096;

enum class BNodeType : u16 {
    ROOT = 0,
    INTERNAL = 1,
    LEAF = 2,
};

#endif