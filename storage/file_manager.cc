#include "file_manager.h"

FileManager::FileManager(const std::string &path) { this->fd = open(path.c_str(), O_RDWR | O_CREAT, 0644); }

FileManager::~FileManager() { close(this->fd); }

void FileManager::read(std::uint64_t offset, void *buffer, std::size_t size) {
  std::size_t total = 0;

  while (total < size) {
    ssize_t n = pread(this->fd, (char *)(buffer) + total, size - total, offset + total);

    total += n;
  }
}

void FileManager::write(std::uint64_t offset, const void *buffer, std::size_t size) {
  std::size_t total = 0;

  while (total < size) {
    ssize_t n = pwrite(this->fd, (const char *)(buffer) + total, size - total, offset + total);

    total += n;
  }
}

void FileManager::sync() { fsync(this->fd); }