#ifndef PPU_H
#define PPU_H

#include <stdint.h>

#define PPU_VRAM_SIZE 0x4000

typedef struct PPU {
    // PPU registers
    uint8_t control;
    uint8_t mask;
    uint8_t status;

    // Sprite memory
    uint8_t oam_addr;
    uint8_t oam[256];

    // Scroll registers
    uint8_t scroll_x;
    uint8_t scroll_y;

    // Shared write latch for PPUSCROLL and PPUADDR
    uint8_t scroll_latch;

    // VRAM address registers
    uint16_t vram_addr;

    // PPUDATA read buffer
    uint8_t data_buffer;

    // Temporary PPU memory
    uint8_t vram[PPU_VRAM_SIZE];

} PPU;

void ppu_init(PPU* ppu);

uint8_t ppu_cpu_read(PPU* ppu, uint16_t address);
void ppu_cpu_write(PPU* ppu, uint16_t address, uint8_t value);

#endif
