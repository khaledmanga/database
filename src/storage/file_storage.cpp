#include "file_storage.hpp"

#include <fcntl.h>
#include <unistd.h>

ssize_t readFile(const char *path, void *buffer, size_t size, off_t offset) {
  int file_fd = open(path, O_RDONLY, 0644);
  return pread(file_fd, buffer, size, offset);
}

ssize_t writeFile(const char *path, const void *buffer, size_t size, off_t offset) {
  int file_fd = open(path, O_WRONLY | O_CREAT, 0644);
  return pwrite(file_fd, buffer, size, offset);
}