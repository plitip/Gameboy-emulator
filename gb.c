#include "gb.h"
#include <stdlib.h>
#include "gb_internal.h"

gb *gb_create(const uint8_t *rom, size_t len){
    gb *g;
    g = calloc(1,sizeof(gb));
    if (g == NULL){
        return NULL;
    } else {
    g->pc=0x0100;
    g->sp = 0xFFFE;
    g->A  = 0x01;
    g->F  = 0xB0;
    g->C  = 0x13;
    g->E  = 0xD8;
    g->H  = 0x01;
    g->L  = 0x4D;
        return g;
    }

}
void gb_destroy(gb *g){
    free(g);
}

void   gb_run_frame(gb *g){
    for(int row =0;144>row;row++)
        for(int col=0;160>col;col++)
            g->screen[row * 160 +col ]=0;
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