#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <fcntl.h>
#include <unistd.h>

class FileManager {
public:
    FileManager(const std::string &path);
    ~FileManager();

    FileManager(const FileManager &) = delete;
    FileManager &operator=(const FileManager &) = delete;

    void read(std::uint64_t offset, void *buffer, std::size_t size);
    void write(std::uint64_t offset, const void *buffer, std::size_t size);

    void sync();

private:
    int fd{-1};
};