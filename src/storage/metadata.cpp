#include "metadata.hpp"

Page convertMetadataToPage(const Metadata &metadata) {
  Page page(PAGE_SIZE, 0);

  unsigned int offset = 0;

  page[offset++] = metadata.root_page_id;
  page[offset++] = metadata.next_page_id;

  return page;
}

Metadata convertPageToMetadata(const Page &page) {
  Metadata metadata;

  unsigned int offset = 0;

  metadata.root_page_id = page[offset++];
  metadata.next_page_id = page[offset++];

  return metadata;
}