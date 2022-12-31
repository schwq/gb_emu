#ifndef INCLUDE_HEADER
#define INCLUDE_HEADER

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef std::string string;

#define NEWLINE "\n";
#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on) { if (on) a |= (1 << n); else a &= ~(1 << n);}
#define DEBUG

#endif // INCLUDE_HEADER