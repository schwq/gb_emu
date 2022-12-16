#include "system.hpp"

template<typename U>
void SYSTEM::LogError(const char* msg, U error) noexcept {
    if(error == NULL) {
        std::cout << msg << std::endl;
    } else {
        std::cout << msg << " " << error << std::endl;
    }
}