#include "file_manager.h"
#include <iostream>

FileManager::FileManager() { this->file_fd = open("database.db", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); }

FileManager::~FileManager() {
  if (this->file_fd != -1) {
    close(this->file_fd);
  }
}

FileManager &FileManager::getInstance() {
  static FileManager instance;
  return instance;
}

void FileManager::writeFile(const void *data, size_t size) {
  if (this->file_fd != -1) {
    write(this->file_fd, (const char *)data, size);
    fsync(this->file_fd);
  }
}

void FileManager::readFile(void *data, size_t size) {
  if (this->file_fd != -1) {
    read(this->file_fd, (char *)data, size);
  }
}

Page FileManager::readPage(u32 id) {
  Page page(PAGE_SIZE);

  ssize_t bytes_read = pread(this->file_fd, (char *)page.data(), PAGE_SIZE, (off_t)id * PAGE_SIZE);

  if (bytes_read < PAGE_SIZE) {
    if (bytes_read < 0) {
      bytes_read = 0;
    }
    page.resize(bytes_read);
  }

  return page;
}

bool FileManager::writePage(u32 id, const Page &page) {
  if (this->file_fd == -1)
    return false;

  ssize_t bytes_write = pwrite(this->file_fd, (const char *)page.data(), PAGE_SIZE, (off_t)id * PAGE_SIZE);

  if (bytes_write < PAGE_SIZE) {
    return false;
  }

  return true;
}

u64 FileManager::allocatePage() {
  Metadata metadata = decodeMetadata(this->readPage(0));
  u64 next_page_id = metadata.next_page_id;
  metadata.updateNextPageId();

  return next_page_id;
}

Metadata FileManager::readMetadata() { return decodeMetadata(this->readPage(0)); }

bool FileManager::writeMetadata(const Metadata &metadata) {
  if (!this->writePage(0, encodeMetadata(metadata))) {
    return false;
  }

  return true;
}
