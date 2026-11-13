#ifndef STORAGE_FILE_STORAGE_HPP
#define

#include <unistd.h>

ssize_t readFile(const char* path, void *buffer, size_t size, off_t offset);
ssize_t writeFile(const void* buffer, size_t size, off_t offset);

#endif