#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>

namespace PRL {

    class Logging {
    public:
        Logging() = delete; // Prevent instantiation of this class

        static void setLogStream(std::ostream& logstream);
        static void setErrStream(std::ostream& errstream);

        static void log(const std::string& message, const std::string& codelocation="");
        static void err(const std::string& message, const std::string& codelocation="");

        static void disableLogging() noexcept;
        static void disableErrLogging() noexcept;

    private:
        static std::string timeStamp_();
        static std::ostream* logstream_;
        static std::ostream* errstream_;
    };

} // namespace PRL

#endif // LOGGING_HPP