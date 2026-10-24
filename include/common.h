#pragma once

#include <stdint.h>
#include <vector>

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

/**
 * Calculates the maximum number of children (d) that can fit in a single node.
 *
 * @note Layout definition in bytes:
 * - Metadata    : Type (2B) + NKeys (2B) = 4B
 * - Pointers    : d * 8B
 * - Offsets     : (d - 1) * 2B
 * - Key Sizes   : (d - 1) * 2B
 * - Value Sizes : (d - 1) * 2B
 *
 * @formula
 *   4 + 8d + 6(d - 1) <= Page_Size
 *   14d - 2 <= 4096  =>  14d <= 4098  =>  d <= 292.71
 *   Result: d = 292 children (291 keys)
 *
 * @param {number} pageSize - The size of the disk page (e.g., 4096).
 * @returns {number} The maximum degree 'd' of the node.
 */

const int MAXIMUM_CHILDREN_IN_ONE_NODE = 292;