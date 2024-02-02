#include "file_manager.h"
#include <iostream>
#include <fstream>
#include <filesystem>

FileManager::FileManager() {
    current_directory = std::filesystem::current_path().string();
}

void FileManager::showContents() {
    std::filesystem::directory_iterator iter(current_directory);
    for (const auto& entry : iter) {
        std::cout << entry.path().filename() << std::endl;
    }
}

void FileManager::createFolder(const std::string& folderName) {
    std::filesystem::create_directory(current_directory + "/" + folderName);
}

void FileManager::createFile(const std::string& fileName) {
    std::ofstream file(current_directory + "/" + fileName);
    file.close();
}

void FileManager::deleteFolder(const std::string& folderName) {
    std::filesystem::remove_all(current_directory + "/" + folderName);
}

void FileManager::deleteFile(const std::string& fileName) {
    std::filesystem::remove(current_directory + "/" + fileName);
}

void FileManager::renameFolder(const std::string& oldName, const std::string& newName) {
    std::filesystem::rename(current_directory + "/" + oldName, current_directory + "/" + newName);
}

void FileManager::renameFile(const std::string& oldName, const std::string& newName) {
    std::filesystem::rename(current_directory + "/" + oldName, current_directory + "/" + newName);
}

void FileManager::copyFolder(const std::string& source, const std::string& destination) {
    std::filesystem::copy(current_directory + "/" + source, current_directory + "/" + destination,
        std::filesystem::copy_options::recursive);
}

void FileManager::moveFolder(const std::string& source, const std::string& destination) {
    std::filesystem::rename(current_directory + "/" + source, current_directory + "/" + destination);
}

int FileManager::calculateSize(const std::string& folderName) {
    std::filesystem::path path_to_check = current_directory + "/" + folderName;
    uintmax_t size = 0;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path_to_check)) {
        if (std::filesystem::is_regular_file(entry.path())) {
            size += std::filesystem::file_size(entry.path());
        }
    }

    return static_cast<int>(size);
}

void FileManager::search(const std::string& pattern) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(current_directory)) {
        if (entry.path().filename().string().find(pattern) != std::string::npos) {
            std::cout << entry.path() << std::endl;
        }
    }
}