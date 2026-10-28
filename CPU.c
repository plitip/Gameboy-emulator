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
    uint8_t opcode = CPUread(g, g->pc);
    g->pc = g->pc + 1;

    switch(opcode){
    case 0x00:
    //where the operation goes but 0x00 is a chud and does nothing.
    return 4; //return how many ticks it took for gb_run_frame()

    /* LD r, n */
    case 0x06:
    g->B = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;
    case 0x0E:
    g->C = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;
    case 0x16:
    g->D = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;
    case 0x1E:
    g->E = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;
    case 0x26:
    g->H = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;
    case 0x2E:
    g->L = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;
    case 0x3E:
    g->A = CPUread(g, g->pc);
    g->pc = g->pc + 1;
    return 8;

    /* LD B, r */
    case 0x40: return 4;
    case 0x41: g->B = g->C; return 4;
    case 0x42: g->B = g->D; return 4;
    case 0x43: g->B = g->E; return 4;
    case 0x44: g->B = g->H; return 4;
    case 0x45: g->B = g->L; return 4;
    case 0x46: g->B = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x47: g->B = g->A; return 4;

    /* LD C, r */
    case 0x48: g->C = g->B; return 4;
    case 0x49: return 4;
    case 0x4A: g->C = g->D; return 4;
    case 0x4B: g->C = g->E; return 4;
    case 0x4C: g->C = g->H; return 4;
    case 0x4D: g->C = g->L; return 4;
    case 0x4E: g->C = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x4F: g->C = g->A; return 4;

    /* LD D, r */
    case 0x50: g->D = g->B; return 4;
    case 0x51: g->D = g->C; return 4;
    case 0x52: return 4;
    case 0x53: g->D = g->E; return 4;
    case 0x54: g->D = g->H; return 4;
    case 0x55: g->D = g->L; return 4;
    case 0x56: g->D = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x57: g->D = g->A; return 4;

    /* LD E, r */
    case 0x58: g->E = g->B; return 4;
    case 0x59: g->E = g->C; return 4;
    case 0x5A: g->E = g->D; return 4;
    case 0x5B: return 4;
    case 0x5C: g->E = g->H; return 4;
    case 0x5D: g->E = g->L; return 4;
    case 0x5E: g->E = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x5F: g->E = g->A; return 4;

    /* LD H, r */
    case 0x60: g->H = g->B; return 4;
    case 0x61: g->H = g->C; return 4;
    case 0x62: g->H = g->D; return 4;
    case 0x63: g->H = g->E; return 4;
    case 0x64: return 4;
    case 0x65: g->H = g->L; return 4;
    case 0x66: g->H = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x67: g->H = g->A; return 4;

    /* LD L, r */
    case 0x68: g->L = g->B; return 4;
    case 0x69: g->L = g->C; return 4;
    case 0x6A: g->L = g->D; return 4;
    case 0x6B: g->L = g->E; return 4;
    case 0x6C: g->L = g->H; return 4;
    case 0x6D: return 4;
    case 0x6E: g->L = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x6F: g->L = g->A; return 4;

    /* LD (HL), r  writes to memory at HL */
    case 0x70: CPUwrite(g, (g->H << 8) | g->L, g->B); return 8;
    case 0x71: CPUwrite(g, (g->H << 8) | g->L, g->C); return 8;
    case 0x72: CPUwrite(g, (g->H << 8) | g->L, g->D); return 8;
    case 0x73: CPUwrite(g, (g->H << 8) | g->L, g->E); return 8;
    case 0x74: CPUwrite(g, (g->H << 8) | g->L, g->H); return 8;
    case 0x75: CPUwrite(g, (g->H << 8) | g->L, g->L); return 8;
    /* 0x76 is HALT, not a load -- later */
    case 0x77: CPUwrite(g, (g->H << 8) | g->L, g->A); return 8;

    /* LD A, r */
    case 0x78: g->A = g->B; return 4;
    case 0x79: g->A = g->C; return 4;
    case 0x7A: g->A = g->D; return 4;
    case 0x7B: g->A = g->E; return 4;
    case 0x7C: g->A = g->H; return 4;
    case 0x7D: g->A = g->L; return 4;
    case 0x7E: g->A = CPUread(g, (g->H << 8) | g->L); return 8;
    case 0x7F: return 4;

    case 0x36:  CPUwrite(g, (g->H << 8) | g->L, CPUread(g, g->pc)); g->pc = g->pc + 1; return 12;
    case 0x0A: g->A=CPUread(g, (g->B << 8) | g->C); return 8;
    case 0x1A: g->A=CPUread(g, (g->D << 8) | g->E); return 8;
    case 0x02: CPUwrite(g, (g->B << 8) | g->C, g->A); return 8;
    case 0x12: CPUwrite(g, (g->D << 8) | g->E, g->A); return 8;

    case 0x22: {uint16_t HL=(g->H << 8) | g->L;CPUwrite(g, HL, g->A); HL=HL+1; g->H=HL>>8; g->L = HL&0xFF;}return 8;
    case 0x2A: {uint16_t HL=(g->H << 8) | g->L; g->A = CPUread(g, HL); HL=HL+1; g->H=HL>>8; g->L = HL&0xFF;} return 8;
    case 0x32: {uint16_t HL=(g->H << 8) | g->L; CPUwrite(g, HL, g->A); HL=HL-1; g->H=HL>>8; g->L = HL&0xFF;} return 8;
    case 0x3A: {uint16_t HL=(g->H << 8) | g->L; g->A = CPUread(g, HL); HL=HL-1; g->H=HL>>8; g->L = HL&0xFF;} return 8;

    case 0xEA: {uint8_t first=CPUread(g, g->pc);g->pc = g->pc + 1; ;uint8_t second =CPUread(g, g->pc);g->pc = g->pc + 1; uint16_t Both=(second << 8) | first; CPUwrite(g,Both,g->A);} return 16;
    case 0xFA: {uint8_t first=CPUread(g, g->pc); g->pc = g->pc + 1; uint8_t second=CPUread(g, g->pc); g->pc = g->pc + 1; uint16_t Both=(second << 8) | first; g->A = CPUread(g, Both);} return 16;
    case 0xE0: {uint8_t n=CPUread(g, g->pc); g->pc = g->pc + 1; CPUwrite(g, 0xFF00 + n, g->A);} return 12;
    case 0xF0: {uint8_t n=CPUread(g, g->pc); g->pc = g->pc + 1; g->A = CPUread(g, 0xFF00 + n);} return 12;
    case 0xE2: CPUwrite(g, 0xFF00 + g->C, g->A); return 8;
    case 0xF2: g->A = CPUread(g, 0xFF00 + g->C); return 8;
    default:
    printf("unknown opcode %02X at %04X\n", opcode, g->pc - 1); //check
    return 4;

    }
}