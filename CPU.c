#include "gb.h"
#include <stdlib.h>

uint8_t CPUread(gb*g, uint16_t x){
return g->memory[x];
}
void CPUwrite(gb*g, uint16_t x, uint8_t y){
g->memory[x] = y;
}