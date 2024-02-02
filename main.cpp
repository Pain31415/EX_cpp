#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

enum class ConsoleColor {
    DEFAULT = 7,
    RED = 12,
    GREEN = 10,
    BLUE = 9
};

class FileExplorer {
public:
    void setConsoleColor(ConsoleColor color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, static_cast<int>(color));
    }

    void displayContents(const fs::path& path) {
        setConsoleColor(ConsoleColor::BLUE);
        std::cout << "Content of " << path << ":\n";
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << entry.path().filename() << std::endl;
        }
        std::cout << std::endl;
        setConsoleColor(ConsoleColor::DEFAULT);
    }

    void changeDrive() {
        std::cout << "Enter the drive letter to switch to (or 'Q' to quit): ";
        std::string driveLetter;
        std::cin >> driveLetter;

        if (driveLetter == "Q" || driveLetter == "q") {
            exit(0);
        }

        fs::path newDrivePath(driveLetter + ":/");
        if (fs::exists(newDrivePath) && fs::is_directory(newDrivePath)) {
            displayContents(newDrivePath);
        }
        else {
            std::cout << "Drive " << driveLetter << " is not available or is not a directory.\n";
            changeDrive();
        }
    }

    void copyFile(const std::string& sourcePath, const std::string& destinationPath) {
        try {
            fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
            setConsoleColor(ConsoleColor::GREEN);
            std::cout << "File copied successfully.\n";
            setConsoleColor(ConsoleColor::DEFAULT);
        }
        catch (const std::exception& ex) {
            setConsoleColor(ConsoleColor::RED);
            std::cerr << "Error copying file: " << ex.what() << std::endl;
            setConsoleColor(ConsoleColor::DEFAULT);
        }
    }

    void copyFolder(const std::string& sourcePath, const std::string& destinationPath) {
        try {
            fs::copy(sourcePath, destinationPath, fs::copy_options::recursive);
            setConsoleColor(ConsoleColor::GREEN);
            std::cout << "Folder copied successfully.\n";
            setConsoleColor(ConsoleColor::DEFAULT);
        }
        catch (const std::exception& ex) {
            setConsoleColor(ConsoleColor::RED);
            std::cerr << "Error copying folder: " << ex.what() << std::endl;
            setConsoleColor(ConsoleColor::DEFAULT);
        }
    }

    void moveFileOrFolder(const std::string& sourcePath, const std::string& destinationPath) {
        try {
            fs::rename(sourcePath, destinationPath);
            setConsoleColor(ConsoleColor::GREEN);
            std::cout << "File/Folder moved successfully.\n";
            setConsoleColor(ConsoleColor::DEFAULT);
        }
        catch (const std::exception& ex) {
            setConsoleColor(ConsoleColor::RED);
            std::cerr << "Error moving file/folder: " << ex.what() << std::endl;
            setConsoleColor(ConsoleColor::DEFAULT);
        }
    }

    void navigateSubfolder(const fs::path& parentPath) {
        while (true) {
            std::cout << "Enter the name of the subfolder to view its contents, or 'B' to go back: ";
            std::string subfolderName;
            std::cin >> subfolderName;

            if (subfolderName == "B" || subfolderName == "b") {
                break;
            }

            fs::path subfolderPath = parentPath / subfolderName;

            if (!fs::exists(subfolderPath)) {
                std::cout << "Subfolder " << subfolderName << " does not exist. Do you want to create it? (Y/N): ";
                char createChoice;
                std::cin >> createChoice;

                if (createChoice == 'Y' || createChoice == 'y') {
                    fs::create_directory(subfolderPath);
                    setConsoleColor(ConsoleColor::BLUE);
                    std::cout << "Subfolder " << subfolderName << " created successfully.\n";
                    setConsoleColor(ConsoleColor::DEFAULT);
                }
                else {
                    std::cout << "Operation canceled.\n";
                    continue;
                }
            }

            std::cout << "Content of " << subfolderPath << ":\n";
            if (fs::exists(subfolderPath) && fs::is_directory(subfolderPath)) {
                displayContents(subfolderPath);

                char choice;
                std::cout << "Do you want to create a file, delete a file/directory, copy/move a file/directory, or go back? (C/D/CP/MV/B): ";
                std::cin >> choice;
                switch (choice) {
                case 'C':
                case 'c':
                {
                    std::string fileName;
                    std::cout << "Enter the name of the file to create: ";
                    std::cin >> fileName;
                    fs::path filePath = subfolderPath / fileName;
                    std::ofstream newFile(filePath.string());
                    newFile.close();
                    setConsoleColor(ConsoleColor::BLUE);
                    std::cout << "File created successfully.\n";
                    setConsoleColor(ConsoleColor::DEFAULT);
                    break;
                }
                case 'D':
                case 'd':
                {
                    std::string itemName;
                    std::cout << "Enter the name of the file/directory to delete: ";
                    std::cin >> itemName;
                    fs::path itemPath = subfolderPath / itemName;
                    if (fs::exists(itemPath)) {
                        if (fs::is_directory(itemPath)) {
                            fs::remove_all(itemPath);
                            setConsoleColor(ConsoleColor::RED);
                            std::cout << "Directory deleted successfully.\n";
                            setConsoleColor(ConsoleColor::DEFAULT);
                        }
                        else {
                            fs::remove(itemPath);
                            setConsoleColor(ConsoleColor::RED);
                            std::cout << "File deleted successfully.\n";
                            setConsoleColor(ConsoleColor::DEFAULT);
                        }
                    }
                    else {
                        std::cout << "File/directory does not exist.\n";
                    }
                    break;
                }
                case 'CP':
                case 'cp':
                {
                    std::string itemName;
                    std::cout << "Enter the name of the file/directory to copy: ";
                    std::cin >> itemName;
                    fs::path itemPath = subfolderPath / itemName;
                    if (fs::exists(itemPath)) {
                        std::string destinationPath;
                        std::cout << "Enter the destination path: ";
                        std::cin >> destinationPath;
                        if (fs::is_directory(itemPath)) {
                            copyFolder(itemPath.string(), destinationPath);
                        }
                        else {
                            copyFile(itemPath.string(), destinationPath);
                        }
                    }
                    else {
                        std::cout << "File/directory does not exist.\n";
                    }
                    break;
                }
                case 'MV':
                case 'mv':
                {
                    std::string itemName;
                    std::cout << "Enter the name of the file/directory to move: ";
                    std::cin >> itemName;
                    fs::path itemPath = subfolderPath / itemName;
                    if (fs::exists(itemPath)) {
                        std::string destinationPath;
                        std::cout << "Enter the destination path: ";
                        std::cin >> destinationPath;
                        moveFileOrFolder(itemPath.string(), destinationPath);
                    }
                    else {
                        std::cout << "File/directory does not exist.\n";
                    }
                    break;
                }
                default:
                    std::cout << "Invalid choice.\n";
                }
            }
            else {
                std::cout << "Subfolder " << subfolderName << " does not exist or is not a directory.\n";
            }
            std::cout << std::endl;
        }
    }
};

void printPenguinHello() {
    std::cout << R"(
     _______
< Hello >
 -------
    \
     \
                                   .--.
                                  |o_o |
                                  |:_/ |
                                 //   \ \
                                (|     | )
                               /'\_   _/`\
                               \___)=(___/
)" << std::endl;
}

void printPenguinGoodbye() {
    std::cout << R"(
     _______
< Goodbye >
 -------
    \
     \
                                   .--.
                                  |o_o |
                                  |:_/ |
                                 //   \ \
                                (|     | )
                               /'\_   _/`\
                               \___)=(___/
)" << std::endl;
}

int main() {
    try {
        FileExplorer fileExplorer;
        fileExplorer.setConsoleColor(ConsoleColor::DEFAULT);
        printPenguinHello();
        std::cout << "Available drives on your computer:\n";

        fs::path rootPath("/");
        for (const auto& entry : fs::directory_iterator(rootPath)) {
            if (fs::is_directory(entry) && entry.path().has_root_name()) {
                std::cout << entry.path().root_name() << "\n";
            }
        }
        std::cout << std::endl;

        while (true) {
            std::cout << "Enter the drive letter to view its contents (e.g., C or D) or 'Q' to quit: ";
            std::string driveLetter;
            std::cin >> driveLetter;

            if (driveLetter == "Q" || driveLetter == "q") {
                break;
            }

            fs::path drivePath(driveLetter + ":/");

            if (fs::exists(drivePath) && fs::is_directory(drivePath)) {
                std::cout << "Content of drive " << driveLetter << ":\n";
                fileExplorer.displayContents(drivePath);

                std::cout << "Enter the folder name to view its contents: ";
                std::string folderName;
                std::cin >> folderName;

                fs::path folderPath = drivePath / folderName;
                if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
                    fileExplorer.displayContents(folderPath);

                    char choice;
                    std::cout << "Do you want to create, delete, copy/move a file/directory, or navigate to a subfolder? (C/D/CP/MV/N): ";
                    std::cin >> choice;
                    switch (choice) {
                    case 'C':
                    case 'c':
                    case 'D':
                    case 'd':
                    case 'CP':
                    case 'cp':
                    case 'MV':
                    case 'mv':
                    case 'N':
                    case 'n':
                        fileExplorer.navigateSubfolder(folderPath);
                        break;
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
        }

        printPenguinGoodbye();
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return 1;
    }
}