#include "file_manager.h"

std::fstream FileManager::fileStream;

FileManager::~FileManager() {
	this->close();
}

void FileManager::close() {
	fileStream.close();
}

void FileManager::open(const string* file_path) {
	fileStream.open(file_path, std::ios::out | std::ios::app);
}

void FileManager::read(int offset, void* buffer, size_t size) {
	fileStream.seekg(offset, std::ios::beg);

	fileStream.read((char*)buffer, size);
}

void FileManager::write(int offset, const void* buffer, size_t size){
	fileStream.seekg(offset);

	fileStream.write((const char*)buffer, size);
}
