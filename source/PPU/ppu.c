
#include "ppu.h"
#include <string.h>

void ppu_init(PPU* ppu) {
    memset(ppu, 0, sizeof(PPU));
}

static void ppu_increment_vram_address(PPU* ppu) {
    if (ppu->control & 0x04) {
        ppu->vram_addr += 32;
    } else {
        ppu->vram_addr += 1;
    }

    ppu->vram_addr &= 0x3FFF;
}

uint8_t ppu_cpu_read(PPU* ppu, uint16_t address) {
    uint16_t reg = address & 0x0007;

    switch (reg) {
        case 0:
            return ppu->control;

        case 1:
            return ppu->mask;

        case 2: {
            uint8_t value = ppu->status;

            // Reading PPUSTATUS clears VBlank and resets the write latch.
            ppu->status &= 0x7F;
            ppu->scroll_latch = 0;

            return value;
        }

        case 3:
            return ppu->oam_addr;

        case 4:
            return ppu->oam[ppu->oam_addr];

        case 5:
            return 0;

        case 6:
            return 0;

        case 7: {
            uint8_t value = ppu->data_buffer;

            ppu->data_buffer =
                ppu->vram[ppu->vram_addr & 0x3FFF];

            ppu_increment_vram_address(ppu);

            return value;
        }

        default:
            return 0;
    }
}

void ppu_cpu_write(PPU* ppu, uint16_t address, uint8_t value) {
    uint16_t reg = address & 0x0007;

    switch (reg) {
        case 0:
            ppu->control = value;
            break;

        case 1:
            ppu->mask = value;
            break;

        case 2:
            // PPUSTATUS is read-only.
            break;

        case 3:
            ppu->oam_addr = value;
            break;

        case 4:
            ppu->oam[ppu->oam_addr] = value;
            ppu->oam_addr++;
            break;

        case 5:
            if (ppu->scroll_latch == 0) {
                ppu->scroll_x = value;
                ppu->scroll_latch = 1;
            } else {
                ppu->scroll_y = value;
                ppu->scroll_latch = 0;
            }
            break;

        case 6:
            if (ppu->scroll_latch == 0) {
                // First write: high byte of VRAM address.
                ppu->vram_addr =
                    (ppu->vram_addr & 0x00FF) |
                    ((uint16_t)(value & 0x3F) << 8);

                ppu->scroll_latch = 1;
            } else {
                // Second write: low byte of VRAM address.
                ppu->vram_addr =
                    (ppu->vram_addr & 0x3F00) | value;

                ppu->scroll_latch = 0;
            }
            break;

        case 7:
            ppu->vram[ppu->vram_addr & 0x3FFF] = value;
            ppu_increment_vram_address(ppu);
            break;

        default:
            break;
    }
}
