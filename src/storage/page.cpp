#include "page.hpp"
#include "metadata.hpp"

bool writePage(u8 page_id, const Page &page) {
  return writeFile("database.db", page.data(), PAGE_SIZE, PAGE_SIZE * page_id) ==
         static_cast<ssize_t>(PAGE_SIZE);
}

Page readPage(u8 page_id) {
  Page page(PAGE_SIZE, 0);

  readFile("database.db", page.data(), PAGE_SIZE, PAGE_SIZE * page_id);

  return page;
}

u8 allocatePage() {
  Metadata metadata = convertPageToMetadata(readPage(0));

  u8 allocated_page = metadata.next_page_id;

  metadata.next_page_id += 1;

  writePage(0, convertMetadataToPage(metadata));

  return allocated_page;
}