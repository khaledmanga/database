#ifndef INCLUDE_COMMON_HPP
#define

#include <cstdint>
#include <vector>

#define std::uint8_t u8
#define std::uint16_t u16
#define std::uint32_t u32
#define std::uint64_t u64

using Page = std::vector<u8>;

const PAGE_SIZE = 4096;

enum class BNodeType {
    ROOT,
    INTERNAL,
    LEAF,
};

#endif