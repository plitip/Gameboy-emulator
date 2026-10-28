#include "gb.h"
#include <stdlib.h>
#include <stdio.h>
uint8_t CPUread(gb*g, uint16_t x){
return g->memory[x];
}
void CPUwrite(gb*g, uint16_t x, uint8_t y){
g->memory[x] = y;
}   

uint8_t CPUstep(gb*g){
    uint8_t opcode= CPUread(g, g->pc);
    g->pc=g->pc+1;  
    switch(opcode){  
    case 0x00:

    //where the operation goes but 0x00 is a chud and does nothing.

    return 4; //return how many ticks it took for gb_run_frame()
    
    case 0x06:
    g->B=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    case 0x0E:
    g->C=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    case 0x16:
    g->D=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    case 0x1E:
    g->E=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    case 0x26:
    g->H=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    case 0x2E:
    g->L=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    case 0x3E:
    g->A=CPUread(g,g->pc);
    g->pc=g->pc+1; 
    return 8;
    default:
    printf("%02X%04x\n", opcode, g->pc-1); //check
    return 4;
    }
}