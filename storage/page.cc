#include "page.h"

Pager::Pager(FileManager &file) : file(file) {}

Page Pager::readPage(u64 page_id) {
  Page page;

  u64 offset = page_id * PAGE_SIZE;

  this->file.read(offset, page.data(), PAGE_SIZE);

  return page;
}

void Pager::writePage(u64 page_id, const Page &page) {
  u64 offset = page_id * PAGE_SIZE;

  this->file.write(offset, page.data(), PAGE_SIZE);
}