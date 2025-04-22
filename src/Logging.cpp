#include <iostream>
#include <algorithm>
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>
#include "Logging.hpp"
#include <SDL2/SDL.h>

namespace PRL {

    std::ostream* Logging::logstream_ = &std::cout;
    std::ostream* Logging::errstream_ = &std::cerr;

    void Logging::setLogStream(std::ostream& logstream) {
        logstream_ = &logstream;
    }
    
    void Logging::setErrStream(std::ostream& errstream) {
        errstream_ = &errstream;
    }

    void Logging::log(const std::string& message, const std::string& codelocation) {
        if (logstream_ == nullptr)
            return;

        *logstream_ << "[LOG]   " << timeStamp_() << " - ";
        if (!codelocation.empty()) {
            *logstream_ << codelocation << " : ";
        }
        *logstream_ << message << std::endl;
    }

    void Logging::err(const std::string& message, const std::string& codelocation) {
        if (errstream_ == nullptr)
            return;

        *errstream_ << "[ERROR] " << timeStamp_() << " - ";
        if (!codelocation.empty()) {
            *errstream_ << codelocation << " : ";
        }
        *errstream_ << message << std::endl;
        SDL_Delay(2000); // Temporary delay for error visibility
        exit(EXIT_FAILURE);
    }

    void Logging::disableLogging() noexcept {
        logstream_ = nullptr;
    }

    void Logging::disableErrLogging() noexcept {
        errstream_ = nullptr;
    }

    std::string Logging::timeStamp_() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto localTime = std::localtime(&time);
        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
        std::string timeStr = oss.str();
        timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), '\n'), timeStr.end());
        return timeStr;
    }

}