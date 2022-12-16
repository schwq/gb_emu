#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

#include "include.hpp"

class SYSTEM {

public:

    template<typename U = u8>
    void LogError(const char* msg, U error = NULL) noexcept;

};

#endif // SYSTEM_HEADER