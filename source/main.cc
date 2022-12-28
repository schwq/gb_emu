#include "include.hpp"
#include "system.hpp"
#include "cartridge.hpp"
#include "controlUnit/cpu.hpp"
#include "ram.hpp"

// static SDL_Window* window = NULL;
static int width = 1080;
static int height = 540;
static u32 pixels[160 * 144];
static RAM ram;
static CARTRIDGE car(ram);
static SYSTEM sys;

int main(int argc, const char** argv) {
    std::string error = "ERROR";

    sys.LogMsg("ERROR ", error);

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
    if(car.loadCartridge("roms/pokemon.gb")) {
        for(int x = 0; x < 0x0100; x++) {
            std::cout << ram.readU8(x) << ", " << std::endl;
        }
    }

    

    return 0;

}