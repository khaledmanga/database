#include "file_manager.h"

FileManager::FileManager() {
  this->file_fd = open("database.db", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
}

FileManager::~FileManager() { delete this->instance; }

void *FileManager::getInstance() {
  static FileManager instance;
  return instance;
}

void FileManager::write(const void *data, size_t size) {
  if (this->file_fd != -1) {
    write(file_fd, (const char *)data, size);
    fsync(this->file_fd);
  }
}

void FileManager::read(void *data, size_t size) {
  if (this->file_fd != -1) {
    read(this->file_fd, (char *)data, size);
  }
}

Page FileManager::readPage(u32 id) {
  Page page{};

  ssize_t bytes_read =
      pread(this->file_fd, (char *)page.data(), PAGE_SIZE, id * PAGE_SIZE);
}

bool FileManager::writePage(u32 id, const Page &page) {
  ssize_t bytes_write = pwrite(this->file_fd, (const char *)page.data(),
                               PAGE_SIZE, id * PAGE_SIZE);

  if (bytes_write < PAGE_SIZE) {
    return false;
  }

  return true;
}