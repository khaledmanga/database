#include "metadata.h"

Metadata::Metadata() {
  this->magic_number = DATABASE_NUMBER;
  this->page_size = PAGE_SIZE;
  this->version = VERSION;
  this->root_page_id = 0;
  this->next_page_id = 1;
}

void Metadata::setRootPageId(u64 root_page_id) { this->root_page_id = root_page_id; }

void Metadata::setNextPageId(u64 next_page_id) { this->next_page_id = next_page_id; }

Page Metadata::encode() {
  Page page{};

  int offset = 0;

  for (int i = 0; i < 8; ++i) {
    page[offset++] = (this->magic_number >> (i * 8)) & 0xFF;
  }

  page[offset++] = this->version;

  page[offset++] = this->page_size & 0xFF;
  page[offset++] = this->page_size >> 8 & 0xFF;

  for (int i = 0; i < 8; ++i) {
    page[offset++] = (this->root_page_id >> (i * 8)) & 0xFF;
  }

  for (int i = 0; i < 8; ++i) {
    page[offset++] = (this->next_page_id >> (i * 8)) & 0xFF;
  }

  return page;
}

u64 Metadata::getRootPageId() const { return this->root_page_id; }