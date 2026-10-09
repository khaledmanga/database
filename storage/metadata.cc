#include "metadata.h"

Metadata::Metadata() {
  this->magic_number = DATABASE_NUMBER;
  this->page_size = PAGE_SIZE;
  this->version = VERSION;
  this->root_page_id = 1;
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

Metadata Metadata::decode(Page &page) {
  Metadata metadata;

  int offset = 0;

  u64 magic_number = 0;

  for (int i = 0; i < 8; ++i) {
    magic_number |= (u64)page[offset++] << (i * 8);
  }

  metadata.magic_number = magic_number;

  metadata.version = page[offset++];

  metadata.page_size = u16(page[offset + 1] << 8 | page[offset]);
  offset += 2;

  u64 root_page_id = 0;
  for (int i = 0; i < 8; ++i) {
    root_page_id |= (u64)page[offset++] << (i * 8);
  }

  u64 next_page_id = 0;
  for (int i = 0; i < 8; ++i) {
    next_page_id |= (u64)page[offset++] << (i * 8);
  }

  metadata.root_page_id = root_page_id;
  metadata.next_page_id = next_page_id;

  return metadata;
}

u64 Metadata::getRootPageId() const { return this->root_page_id; }

u64 Metadata::getMagicNumber() const { return this->magic_number; }

u64 Metadata::getNextPageId() const { return this->next_page_id; }

void Metadata::update_metadata(Pager &pager) {
  ++this->next_page_id;

  pager.writePage(0, this->encode());
}