#ifndef PPU_H
#define PPU_H

#include <stdint.h>

#define PPU_CHR_SIZE 0x2000
#define PPU_NAMETABLE_SIZE 0x1000
#define PPU_PALETTE_SIZE 0x20

typedef enum {
    MIRRORING_HORIZONTAL,
    MIRRORING_VERTICAL,
    MIRRORING_FOUR_SCREEN
} Mirroring;

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
    uint8_t scroll_latch;

    // VRAM address registers
    uint16_t vram_addr;

    // PPUDATA read buffer
    uint8_t data_buffer;

    // PPU memory
    uint8_t chr[PPU_CHR_SIZE];
    uint8_t nametable_ram[PPU_NAMETABLE_SIZE];
    uint8_t palette_ram[PPU_PALETTE_SIZE];

    // CHR and nametable configuration
    uint8_t chr_is_ram;
    Mirroring mirroring;

} PPU;

void ppu_init(PPU* ppu);

uint8_t ppu_cpu_read(PPU* ppu, uint16_t address);
void ppu_cpu_write(PPU* ppu, uint16_t address, uint8_t value);

uint8_t ppu_read_memory(PPU* ppu, uint16_t address);
void ppu_write_memory(PPU* ppu, uint16_t address, uint8_t value);

#endif