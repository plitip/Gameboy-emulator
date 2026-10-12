#include <SDL2/SDL.h>
#include "gb.h"
int main(int argc, char *argv[]){
    gb *g = gb_create(NULL, 0); // placeholder null value for it to compile
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window *window = SDL_CreateWindow("GAMEBOY",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,3*160,3*144,SDL_WINDOW_SHOWN);
    int running = 1;
    for(;running==1;){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {  
    if (event.type == SDL_QUIT){
    running =0;
    break;
    }
    }
}


    SDL_DestroyWindow(window);
}