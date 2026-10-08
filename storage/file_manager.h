#pragma once

#include <iostream>
#include <cstring>

class FileManager {
	private:
		static std::fstream fileStream;
		FileManager() = delete;
		FileManager(const FileManager&) = delete;
		FileManager& operator=(const FileManager&) = delete;
	public:
		~FileManager();

		static void open(const string& file_path);
		static void close();
		static void read(int offset, void* buffer, size_t size);
		static void write(int offset, const void* buffer, size_t size);
};
