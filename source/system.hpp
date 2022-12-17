#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

#include "include.hpp"

class SYSTEM {

public:

    template<typename U>
    void LogError(const U& msg) noexcept;

    template<typename U, typename... T>
    void LogError(const U& u, const T&... t) noexcept;

    template<typename U>
    void LogMsg(const U& msg) noexcept;

    template<typename U, typename... T>
    void LogMsg(const U& u, const T&... t) noexcept;

};

#endif // SYSTEM_HEADER