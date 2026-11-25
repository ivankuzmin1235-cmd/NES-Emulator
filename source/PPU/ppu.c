#include "ppu.h"

#include <string.h>

void ppu_init(PPU* ppu) {
    memset(ppu, 0, sizeof(PPU));
}

uint8_t ppu_cpu_read(PPU* ppu, uint16_t address) {
    uint16_t reg = address & 0x0007;

    return ppu->registers[reg];
}

void ppu_cpu_write(PPU* ppu, uint16_t address, uint8_t value) {
    uint16_t reg = address & 0x0007;

    ppu->registers[reg] = value;
}
