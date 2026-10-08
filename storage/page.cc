#include "page.h"

Metadata::Metadata {
  this->magic_number = DATABASE_NUMBER;
  this->page_size = PAGE_SIZE;
  this->version = VERSION;
}

void Metadata::setRootPageId(u64 root_page_id) {
  this->root_page_id = root_page_id;
}

void Metadata::setNextPageid(u64 next_page_id) {
  this->next_page_id = next_page_id;
}

Page Pager::readPage(u64 page_id) {
  Page page;

  int offset = page_id *  PAGE_SIZE;

  fileStream.read(offset, page.data(), PAGE_SIZE);

  return page;
}

void Pager::writePage(u64 page_id, Page &page) {
  int offset = page_id * PAGE_SIZE;

  fileStream.write(offset, page.data(), PAGE_SIZE);
}

