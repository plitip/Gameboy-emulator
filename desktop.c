#include <SDL2/SDL.h>
#include "gb.h"
SDL_Renderer *renderer;

void draw_term(const uint8_t *screen_par){
SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
SDL_RenderClear(renderer);
SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
    for (int row_counter =0;144>row_counter;row_counter++){
        for (int col_counter =0;160>col_counter;col_counter++){
            if (screen_par[row_counter * 160 + col_counter] == 0) {
            SDL_Rect rect = {col_counter*3, row_counter*3, 3, 3};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
            }
            else if (screen_par[row_counter * 160 + col_counter] == 1) {
            SDL_Rect rect = {col_counter*3, row_counter*3, 3, 3};
            SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
            SDL_RenderFillRect(renderer, &rect);
            }
            else if (screen_par[row_counter * 160 + col_counter] == 2) {
            SDL_Rect rect = {col_counter*3, row_counter*3, 3, 3};
            SDL_SetRenderDrawColor(renderer, 85, 85, 85, 255);
            SDL_RenderFillRect(renderer, &rect);
            } else {
            SDL_Rect rect = {col_counter*3, row_counter*3, 3, 3};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
            }
        
    }
}
}
int main(int argc, char *argv[]){
    gb *g = gb_create(NULL, 0); // placeholder null value for it to compile
    const uint8_t *screen_par=gb_screen(g);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window *window = SDL_CreateWindow("GAMEBOY",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,3*160,3*144,SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1, 0);
    int running = 1;
    for(;running==1;){
    SDL_Event event;
    while (SDL_PollEvent(&event)) { 
    if (event.type == SDL_QUIT){
        
    running =0;
    
    break;
    }
    }
    gb_run_frame(g);
    draw_term(screen_par);
    SDL_RenderPresent(renderer);
}


    SDL_DestroyWindow(window);
}