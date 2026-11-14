#ifndef STORAGE_PAGE_HPP
#define STORAGE_PAGE_HPP

#include "file_storage.hpp"
#include "include/common.hpp"

bool writePage(u8 page_id, const Page &page);
Page readPage(u8 page_id);
u8 allocatePage();

#endif