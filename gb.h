#ifndef gb_h
#define gb_h

#include "gb.h"

#endif
#include "cpu.c"
#include <stdint.h>
#include <stddef.h>

typedef struct gb gb;                            /* the whole machine, opaque */
struct gb {
    uint8_t screen[160*144];
    uint16_t pc;
    uint16_t sp;
    uint8_t A;
    uint8_t F;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    uint8_t memory[65536];
};
gb    *gb_create(const uint8_t *rom, size_t len); /* the only malloc */
void   gb_destroy(gb *g);
void   gb_run_frame(gb *g);                      /* 70,224 ticks = one frame */
const uint8_t *gb_screen(const gb *g);           /* 160×144 shades, 0–3 */
void   gb_set_buttons(gb *g, uint8_t held);      /* bit 0 Right  bit 7 Start */
uint8_t *gb_cart_ram(gb *g, size_t *len);        /* battery RAM .sav file */
void   gb_on_serial(gb *g, void (*fn)(uint8_t, void *), void *ctx);