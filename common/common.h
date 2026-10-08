#pragma once

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define DATABASE_NUMBER 0x4441544142415345ULL

const PAGE_SIZE = 4096;
const VERSION = 1;
const Page = std::vector<u8, PAGE_SIZE>
