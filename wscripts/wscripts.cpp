#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> getExecutableFiles(const std::string& directory) {
    std::vector<std::string> executables;

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string fileName = findFileData.cFileName;
                size_t dotIndex = fileName.find_last_of(".");
                if (dotIndex != std::string::npos) {
                    std::string extension = fileName.substr(dotIndex + 1);
                    if (extension == "exe") {
                        executables.push_back(fileName);
                    }
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return executables;
}

int main() {
    std::vector<std::string> executables = getExecutableFiles("C:\\Users\\Vince\\Documents\\Scripts");

    if (executables.empty()) {
        std::cerr << "No executable files found in the directory.\n";
        return 1;
    }

    std::stringstream ss;
    for (const std::string& exe : executables) {
        ss << exe << ",";
    }

    std::string elements = ss.str();
    elements.pop_back(); // Remove the trailing comma

    std::string command = "wmenu.exe -elements " + elements + " | more";
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to open pipe to command.\n";
        return 1;
    }

    // Read the output of the command
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    _pclose(pipe);

    // Now execute the result
    system(result.c_str());


    return 0;
}

