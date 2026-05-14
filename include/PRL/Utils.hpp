#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <string>

namespace PRL {
    // File path utilities
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

    // Comparison utilities
    template<typename T>
    inline bool inRange(const T& value, const T& min, const T& max) {
        static_assert(std::is_arithmetic<T>::value, "inRange requires an arithmetic type");
        return value >= min && value <= max;
    }

    template<typename T>
    inline bool inRangeStrict(const T& value, const T& min, const T& max) {
        static_assert(std::is_arithmetic<T>::value, "inRangeStrict requires an arithmetic type");
        return value > min && value < max;
    }
}

#endif // UTILS_HPP_INCLUDED