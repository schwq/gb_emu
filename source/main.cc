#include "include.hpp"
#include "system.hpp"
#include "cartridge.hpp"
#include "memory.h"

// static SDL_Window* window = NULL;
static int width = 1080;
static int height = 540;
static u32 pixels[160 * 144];
static MEMORY mem;

int main(int argc, const char** argv) {

    std::cout << "[START]: Loading emulator..." << std::endl;
    /*
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "[ERROR]: SDL2 could not initialize!" << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("GB_EMU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        std::cout << "[ERROR]: Window could not be create!" << std::endl;
        exit(1);
    }
    */
    /*
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, width, height);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        160, 144);
    */

   CARTRIDGE cart(mem);
   if(cart.loadCartridge("roms/pokemon.gb")) {
        std::cout << "LOADED" << std::endl;
   } else {
        std::cout << "NOT LOAD" << std::endl;
   }

    return 0;

}