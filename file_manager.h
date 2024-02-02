#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

class FileManager {
private:
    std::string current_directory;

public:
    FileManager();
    void showContents();
    void createFolder(const std::string& folderName);
    void createFile(const std::string& fileName);
    void deleteFolder(const std::string& folderName);
    void deleteFile(const std::string& fileName);
    void renameFolder(const std::string& oldName, const std::string& newName);
    void renameFile(const std::string& oldName, const std::string& newName);
    void addFileToFolder(const std::string& folderName, const std::string& fileName);
    void moveFolder(const std::string& source, const std::string& destination);
    int calculateSize(const std::string& folderName);
    void search(const std::string& pattern);
};

#endif