#include "gb.h"
#include <stdlib.h>
struct gb {
    uint8_t screen[160*144];
};

gb *gb_create(const uint8_t *rom, size_t len){
    gb *g;
    
    g = calloc(1,sizeof(gb));

    if (g == NULL){
        return NULL;
    } else {
        return g;
    }
}
void gb_destroy(gb *g){
    free(g);
}

void   gb_run_frame(gb *g){
    // no cpu so 0 ticks 
}

const uint8_t *gb_screen(const gb *g){
    return g->screen;
}

void gb_set_buttons(gb *g, uint8_t held){

}

uint8_t *gb_cart_ram(gb *g, size_t *len){

}
void   gb_on_serial(gb *g, void (*fn)(uint8_t, void *), void *ctx){
    
}