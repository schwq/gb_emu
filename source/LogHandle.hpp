#ifndef LOG_HANDLE_HEADER
#define LOG_HANDLE_HEADER
#include "include.hpp"


template<typename U>
void ErrorLogMessage(const U& Message) {
    std::cerr << Message << std::endl;
}

template<typename T, typename... Y>
void ErrorLogMessage(const T& Message, const Y&... Error) {
    std::cerr << Message;
    ErrorLogMessage(Error...);
}

template<typename U>
void LogMessage(const U& Message) {
    std::cout << Message << std::endl;
}

template<typename T, typename... Y>
void LogMessage(const T& Message, const Y&... Rest) {
    std::cout << Message;
    LogMessage(Rest...);
}

#endif