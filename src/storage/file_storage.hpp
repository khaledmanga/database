#ifndef STORAGE_FILE_STORAGE_HPP
#define STORAGE_FILE_STORAGE_HPP

#include <sys/types.h>

ssize_t readFile(const char *path, void *buffer, size_t size, off_t offset);
ssize_t writeFile(const char *path, const void *buffer, size_t size, off_t offset);

#endif