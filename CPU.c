#include "gb.h"
#include <stdlib.h>
#include <stdio.h>
#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

uint8_t CPUread(gb*g, uint16_t x){
    return g->memory[x];
}

void CPUwrite(gb*g, uint16_t x, uint8_t y){
    g->memory[x] = y;
}

/* A = A + value (+ carry). Sets Z, N=0, H, C.
   ADC passes the old carry: (g->F & FLAG_C) != 0 gives 1 if C is on, 0 if not. */
void CPUadd(gb*g, uint8_t value, uint8_t carry){
    uint16_t sum = g->A + value + carry;
    uint8_t half = (g->A & 0xF) + (value & 0xF) + carry;
    g->A = sum & 0xFF;
    g->F = 0;
    if (g->A == 0)   g->F = g->F | FLAG_Z;
    if (half > 0xF)  g->F = g->F | FLAG_H;
    if (sum > 0xFF)  g->F = g->F | FLAG_C;
}

/* A - value (- carry). Sets Z, N=1, H, C. Stores into A only if keep is 1 (CP uses keep = 0). */
void CPUsub(gb*g, uint8_t value, uint8_t carry, uint8_t keep){
    int diff = g->A - value - carry;
    int half = (g->A & 0xF) - (value & 0xF) - carry;
    uint8_t result = diff & 0xFF;
    g->F = FLAG_N;
    if (result == 0) g->F = g->F | FLAG_Z;
    if (half < 0)    g->F = g->F | FLAG_H;
    if (diff < 0)    g->F = g->F | FLAG_C;
    if (keep) g->A = result;
}

uint8_t CPUinc(gb*g, uint8_t value){
    uint8_t result = value + 1;
    g->F = g->F & FLAG_C;
    if (result == 0)         g->F = g->F | FLAG_Z;
    if ((result & 0xF) == 0) g->F = g->F | FLAG_H;
    return result;
}
uint8_t CPUdec(gb*g, uint8_t value){
    uint8_t result = value - 1;
    g->F = (g->F & FLAG_C) | 1;
    if (result == 0)         g->F = g->F | FLAG_Z;
    if ((result & 0xF) == 0) g->F = g->F | FLAG_H;
    return result;
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
    case 0x22: {uint16_t HL=(g->H << 8) | g->L; CPUwrite(g, HL, g->A); HL=HL+1; g->H=HL>>8; g->L = HL&0xFF;} return 8;
    case 0x2A: {uint16_t HL=(g->H << 8) | g->L; g->A = CPUread(g, HL); HL=HL+1; g->H=HL>>8; g->L = HL&0xFF;} return 8;
    case 0x32: {uint16_t HL=(g->H << 8) | g->L; CPUwrite(g, HL, g->A); HL=HL-1; g->H=HL>>8; g->L = HL&0xFF;} return 8;
    case 0x3A: {uint16_t HL=(g->H << 8) | g->L; g->A = CPUread(g, HL); HL=HL-1; g->H=HL>>8; g->L = HL&0xFF;} return 8;

    case 0xEA: {uint8_t first=CPUread(g, g->pc); g->pc = g->pc + 1; uint8_t second=CPUread(g, g->pc); g->pc = g->pc + 1; uint16_t Both=(second << 8) | first; CPUwrite(g, Both, g->A);} return 16;
    case 0xFA: {uint8_t first=CPUread(g, g->pc); g->pc = g->pc + 1; uint8_t second=CPUread(g, g->pc); g->pc = g->pc + 1; uint16_t Both=(second << 8) | first; g->A = CPUread(g, Both);} return 16;
    case 0xE0: {uint8_t n=CPUread(g, g->pc); g->pc = g->pc + 1; CPUwrite(g, 0xFF00 + n, g->A);} return 12;
    case 0xF0: {uint8_t n=CPUread(g, g->pc); g->pc = g->pc + 1; g->A = CPUread(g, 0xFF00 + n);} return 12;
    case 0xE2: CPUwrite(g, 0xFF00 + g->C, g->A); return 8;
    case 0xF2: g->A = CPUread(g, 0xFF00 + g->C); return 8;

    /* AND A, r  -- Z if A is 0, N=0, H=1, C=0 */
    case 0xA0: g->A = g->A & g->B; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xA1: g->A = g->A & g->C; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xA2: g->A = g->A & g->D; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xA3: g->A = g->A & g->E; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xA4: g->A = g->A & g->H; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xA5: g->A = g->A & g->L; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xA6: g->A = g->A & CPUread(g, (g->H << 8) | g->L); if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 8;
    case 0xA7: g->A = g->A & g->A; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 4;
    case 0xE6: g->A = g->A & CPUread(g, g->pc); g->pc = g->pc + 1; if (g->A==0){g->F = FLAG_Z | FLAG_H;}else{g->F = FLAG_H;} return 8;

    /* XOR A, r  -- Z if A is 0, N=0, H=0, C=0 */
    case 0xA8: g->A = g->A ^ g->B; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xA9: g->A = g->A ^ g->C; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xAA: g->A = g->A ^ g->D; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xAB: g->A = g->A ^ g->E; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xAC: g->A = g->A ^ g->H; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xAD: g->A = g->A ^ g->L; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xAE: g->A = g->A ^ CPUread(g, (g->H << 8) | g->L); if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 8;
    case 0xAF: g->A = 0; g->F = FLAG_Z; return 4;
    case 0xEE: g->A = g->A ^ CPUread(g, g->pc); g->pc = g->pc + 1; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 8;

    /* OR A, r  -- Z if A is 0, N=0, H=0, C=0 */
    case 0xB0: g->A = g->A | g->B; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xB1: g->A = g->A | g->C; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xB2: g->A = g->A | g->D; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xB3: g->A = g->A | g->E; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xB4: g->A = g->A | g->H; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xB5: g->A = g->A | g->L; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xB6: g->A = g->A | CPUread(g, (g->H << 8) | g->L); if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 8;
    case 0xB7: g->A = g->A | g->A; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 4;
    case 0xF6: g->A = g->A | CPUread(g, g->pc); g->pc = g->pc + 1; if (g->A==0){g->F = FLAG_Z;}else{g->F = 0;} return 8;

    /* ADD A, r */
    case 0x80: CPUadd(g, g->B, 0); return 4;
    case 0x81: CPUadd(g, g->C, 0); return 4;
    case 0x82: CPUadd(g, g->D, 0); return 4;
    case 0x83: CPUadd(g, g->E, 0); return 4;
    case 0x84: CPUadd(g, g->H, 0); return 4;
    case 0x85: CPUadd(g, g->L, 0); return 4;
    case 0x86: CPUadd(g, CPUread(g, (g->H << 8) | g->L), 0); return 8;
    case 0x87: CPUadd(g, g->A, 0); return 4;

    /* ADC A, r */
    case 0x88: CPUadd(g, g->B, (g->F & FLAG_C) != 0); return 4;
    case 0x89: CPUadd(g, g->C, (g->F & FLAG_C) != 0); return 4;
    case 0x8A: CPUadd(g, g->D, (g->F & FLAG_C) != 0); return 4;
    case 0x8B: CPUadd(g, g->E, (g->F & FLAG_C) != 0); return 4;
    case 0x8C: CPUadd(g, g->H, (g->F & FLAG_C) != 0); return 4;
    case 0x8D: CPUadd(g, g->L, (g->F & FLAG_C) != 0); return 4;
    case 0x8E: CPUadd(g, CPUread(g, (g->H << 8) | g->L), (g->F & FLAG_C) != 0); return 8;
    case 0x8F: CPUadd(g, g->A, (g->F & FLAG_C) != 0); return 4;

    /* SUB A, r */
    case 0x90: CPUsub(g, g->B, 0, 1); return 4;
    case 0x91: CPUsub(g, g->C, 0, 1); return 4;
    case 0x92: CPUsub(g, g->D, 0, 1); return 4;
    case 0x93: CPUsub(g, g->E, 0, 1); return 4;
    case 0x94: CPUsub(g, g->H, 0, 1); return 4;
    case 0x95: CPUsub(g, g->L, 0, 1); return 4;
    case 0x96: CPUsub(g, CPUread(g, (g->H << 8) | g->L), 0, 1); return 8;
    case 0x97: CPUsub(g, g->A, 0, 1); return 4;

    /* SBC A, r */
    case 0x98: CPUsub(g, g->B, (g->F & FLAG_C) != 0, 1); return 4;
    case 0x99: CPUsub(g, g->C, (g->F & FLAG_C) != 0, 1); return 4;
    case 0x9A: CPUsub(g, g->D, (g->F & FLAG_C) != 0, 1); return 4;
    case 0x9B: CPUsub(g, g->E, (g->F & FLAG_C) != 0, 1); return 4;
    case 0x9C: CPUsub(g, g->H, (g->F & FLAG_C) != 0, 1); return 4;
    case 0x9D: CPUsub(g, g->L, (g->F & FLAG_C) != 0, 1); return 4;
    case 0x9E: CPUsub(g, CPUread(g, (g->H << 8) | g->L), (g->F & FLAG_C) != 0, 1); return 8;
    case 0x9F: CPUsub(g, g->A, (g->F & FLAG_C) != 0, 1); return 4;

    /* CP A, r */
    case 0xB8: CPUsub(g, g->B, 0, 0); return 4;
    case 0xB9: CPUsub(g, g->C, 0, 0); return 4;
    case 0xBA: CPUsub(g, g->D, 0, 0); return 4;
    case 0xBB: CPUsub(g, g->E, 0, 0); return 4;
    case 0xBC: CPUsub(g, g->H, 0, 0); return 4;
    case 0xBD: CPUsub(g, g->L, 0, 0); return 4;
    case 0xBE: CPUsub(g, CPUread(g, (g->H << 8) | g->L), 0, 0); return 8;
    case 0xBF: CPUsub(g, g->A, 0, 0); return 4;

    /* the same, with the next byte (n) */
    case 0xC6: {uint8_t n = CPUread(g, g->pc); g->pc = g->pc + 1; CPUadd(g, n, 0);} return 8;
    case 0xCE: {uint8_t n = CPUread(g, g->pc); g->pc = g->pc + 1; CPUadd(g, n, (g->F & FLAG_C) != 0);} return 8;
    case 0xD6: {uint8_t n = CPUread(g, g->pc); g->pc = g->pc + 1; CPUsub(g, n, 0, 1);} return 8;
    case 0xDE: {uint8_t n = CPUread(g, g->pc); g->pc = g->pc + 1; CPUsub(g, n, (g->F & FLAG_C) != 0, 1);} return 8;
    case 0xFE: {uint8_t n = CPUread(g, g->pc); g->pc = g->pc + 1; CPUsub(g, n, 0, 0);} return 8;
    //incs
    case 0x04: g->B = CPUinc(g, g->B); return 4;
    case 0x0C: g->C = CPUinc(g, g->C); return 4;
    case 0x14: g->D = CPUinc(g, g->D); return 4;
    case 0x1C: g->E = CPUinc(g, g->E); return 4;
    case 0x24: g->H = CPUinc(g, g->H); return 4;
    case 0x2C: g->L = CPUinc(g, g->L); return 4;
    case 0x3C: g->A = CPUinc(g, g->A); return 4;
    case 0x34: {uint16_t HL = (g->H << 8) | g->L; CPUwrite(g, HL, CPUinc(g, CPUread(g, HL)));} return 12;
    
    default:
    printf("unknown opcode %02X at %04X\n", opcode, g->pc - 1); //check
    return 4;

    }
}