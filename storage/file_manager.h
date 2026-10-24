#pragma once

#include "include/common.h"
#include "metadata/metadata.h"
#include <fcntl.h>
#include <unistd.h>

class FileManager {
private:
  int file_fd;
  FileManager();

public:
  FileManager(const FileManager &) = delete;
  FileManager &operator=(const FileManager &) = delete;

  ~FileManager();

  static FileManager &getInstance();

  void readFile(void *data, size_t size);
  void writeFile(const void *data, size_t size);

  bool writePage(u32 id, const Page &page);
  Page readPage(u32 id);
  u64 allocatePage();
};
