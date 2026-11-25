#ifndef PPU_H
#define PPU_H

#include <stdint.h>

typedef struct PPU {
    uint8_t registers[8];

} PPU;

void ppu_init(PPU* ppu);

uint8_t ppu_cpu_read(PPU* ppu, uint16_t address);
void ppu_cpu_write(PPU* ppu, uint16_t address, uint8_t value);

#endif