#pragma once

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
  static FileManager *instance = nullptr;

  static FileManager *getInstance();
  void readFile(void *data, size_t size);
  void writeFile(const void *data, size_t size);
};