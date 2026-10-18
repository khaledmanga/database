#pragma once

class FileManager {
    public:
        FileManager();
        ~FileManager();
        int file_fd;
        static FileManager *instance;

        static FileManager getInstance();
        void readFile(void* data, size_t size);
        void writeFile(const void* data, size_t size);
}