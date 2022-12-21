#include "system.hpp"

template<typename U>
void SYSTEM::LogError(const U& msg) noexcept {
    std::cerr << msg << std::endl;
}

template<typename U, typename... T>
void SYSTEM::LogError(const U& u, const T&... t) noexcept {
    std::cerr << u << ", ";
    LogError(t...);
}

template<typename U>
void SYSTEM::LogMsg(const U& msg) noexcept {
    std::cout << msg << std::end;
}

template<typename U, typename... T>
void SYSTEM::LogMsg(const U& u, const T&... t) noexcept {
    std::cout << u << ", ";
    LogMsg(t...);
}

void SYSTEM::logMessage(const char* _msg) noexcept {
    std::cout << _msg << std::endl;
}