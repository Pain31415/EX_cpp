#include <iostream>
#include <boost/filesystem.hpp>

void displayContents(const boost::filesystem::path& path) {
    std::cout << "Content of " << path << ":\n";
    for (const auto& entry : boost::filesystem::directory_iterator(path)) {
        std::cout << entry << std::endl;
    }
}

void createFolder(const boost::filesystem::path& parentPath, const std::string& folderName) {
    boost::filesystem::path folderPath = parentPath / folderName;
    if (!boost::filesystem::exists(folderPath)) {
        if (boost::filesystem::create_directory(folderPath)) {
            std::cout << "Folder " << folderName << " created successfully.\n";
        }
        else {
            std::cout << "Failed to create folder " << folderName << ".\n";
        }
    }
    else {
        std::cout << "Folder " << folderName << " already exists.\n";
    }
}

void deleteFolder(const boost::filesystem::path& parentPath, const std::string& folderName) {
    boost::filesystem::path folderPath = parentPath / folderName;
    if (boost::filesystem::exists(folderPath) && boost::filesystem::is_directory(folderPath)) {
        if (boost::filesystem::remove(folderPath)) {
            std::cout << "Folder " << folderName << " deleted successfully.\n";
        }
        else {
            std::cout << "Failed to delete folder " << folderName << ".\n";
        }
    }
    else {
        std::cout << "Folder " << folderName << " does not exist or is not a directory.\n";
    }
}

int main() {
    std::cout << "Available drives on your computer:\n";

    boost::filesystem::path rootPath("/");
    for (const auto& entry : boost::filesystem::directory_iterator(rootPath)) {
        if (boost::filesystem::is_directory(entry) && entry.path().has_root_name()) {
            std::cout << entry.path().root_name() << "\n";
        }
    }

    std::cout << "Enter the drive letter to view its contents (e.g., C or D): ";
    std::string driveLetter;
    std::cin >> driveLetter;

    boost::filesystem::path drivePath(driveLetter + ":/");

    if (boost::filesystem::exists(drivePath) && boost::filesystem::is_directory(drivePath)) {
        std::cout << "Content of drive " << driveLetter << ":\n";
        displayContents(drivePath);

        std::cout << "Enter the folder name to view its contents: ";
        std::string folderName;
        std::cin >> folderName;

        boost::filesystem::path folderPath = drivePath / folderName;
        if (boost::filesystem::exists(folderPath) && boost::filesystem::is_directory(folderPath)) {
            displayContents(folderPath);

            std::cout << "Do you want to create or delete a folder? (C/D): ";
            char choice;
            std::cin >> choice;
            switch (choice) {
            case 'C':
            case 'c': {
                std::cout << "Enter the name of the new folder: ";
                std::string newFolderName;
                std::cin >> newFolderName;
                createFolder(folderPath, newFolderName);
                break;
            }
            case 'D':
            case 'd': {
                std::cout << "Enter the name of the folder to delete: ";
                std::string deleteFolderName;
                std::cin >> deleteFolderName;
                deleteFolder(folderPath, deleteFolderName);
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
            }
        }
        else {
            std::cout << "Folder " << folderName << " does not exist or is not a directory.\n";
        }
    }
    else {
        std::cout << "Drive " << driveLetter << " is not available or is not a directory.\n";
    }

    return 0;
}