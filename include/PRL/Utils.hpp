#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <string>

std::string getFileExtension(const std::string& fileName) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == std::string::npos) {
        return ""; // No extension found
    }
    return fileName.substr(dotPos + 1);
}

std::string getFileNameWithoutExtension(const std::string& fileName) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == std::string::npos) {
        return fileName; // No extension found, return original name
    }
    return fileName.substr(0, dotPos);
}

#endif // UTILS_HPP_INCLUDED