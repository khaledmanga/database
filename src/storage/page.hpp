#ifndef STORAGE_PAGE_HPP
#define

#include "include/common.hpp"

bool writePage(u8 page_id, const Page& page);
Page readPage(u8 page_id);

#endif