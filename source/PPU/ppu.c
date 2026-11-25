
#include "ppu.h"

#include <string.h>

void ppu_init(PPU* ppu) {
    memset(ppu, 0, sizeof(PPU));

    // Until a ROM is loaded, use CHR RAM.
    ppu->chr_is_ram = 1;

    // Default nametable mirroring.
    ppu->mirroring = MIRRORING_HORIZONTAL;
}

// Increment the VRAM address after PPUDATA access.
static void ppu_increment_vram_address(PPU* ppu) {
    if (ppu->control & 0x04) {
        ppu->vram_addr += 32;
    } else {
        ppu->vram_addr += 1;
    }

    ppu->vram_addr &= 0x3FFF;
}

// Convert a logical nametable address
// into a physical nametable RAM index.
static uint16_t ppu_mirror_nametable_address(
    PPU* ppu,
    uint16_t address
) {
    uint16_t offset = (address - 0x2000) & 0x0FFF;

    uint16_t table = offset / 0x0400;
    uint16_t table_offset = offset & 0x03FF;

    uint16_t physical_table = 0;

    switch (ppu->mirroring) {
        case MIRRORING_VERTICAL:
            physical_table = table & 0x01;
            break;

        case MIRRORING_HORIZONTAL:
            physical_table = table >> 1;
            break;

        case MIRRORING_FOUR_SCREEN:
            physical_table = table;
            break;

        default:
            physical_table = table >> 1;
            break;
    }

    return physical_table * 0x0400 + table_offset;
}

// Mirror special palette addresses.
static uint16_t ppu_mirror_palette_address(uint16_t address) {
    uint16_t index = (address - 0x3F00) & 0x001F;

    // $3F10, $3F14, $3F18 and $3F1C
    // mirror $3F00, $3F04, $3F08 and $3F0C.
    if (index == 0x10 ||
        index == 0x14 ||
        index == 0x18 ||
        index == 0x1C) {
        index -= 0x10;
    }

    return index;
}

// Read from PPU memory.
uint8_t ppu_read_memory(PPU* ppu, uint16_t address) {
    address &= 0x3FFF;

    // CHR memory: $0000-$1FFF
    if (address <= 0x1FFF) {
        return ppu->chr[address];
    }

    // Nametable memory: $2000-$2FFF
    // $3000-$3EFF mirrors $2000-$2EFF.
    if (address >= 0x2000 && address <= 0x3EFF) {
        uint16_t mirrored_address;

        if (address >= 0x3000) {
            address -= 0x1000;
        }

        mirrored_address =
            ppu_mirror_nametable_address(ppu, address);

        return ppu->nametable_ram[mirrored_address];
    }

    // Palette memory: $3F00-$3FFF
    if (address >= 0x3F00) {
        uint16_t index =
            ppu_mirror_palette_address(address);

        return ppu->palette_ram[index] & 0x3F;
    }

    return 0;
}

// Write to PPU memory.
void ppu_write_memory(
    PPU* ppu,
    uint16_t address,
    uint8_t value
) {
    address &= 0x3FFF;

    // CHR memory: $0000-$1FFF
    if (address <= 0x1FFF) {
        if (ppu->chr_is_ram) {
            ppu->chr[address] = value;
        }

        return;
    }

    // Nametable memory: $2000-$2FFF
    // $3000-$3EFF mirrors $2000-$2EFF.
    if (address >= 0x2000 && address <= 0x3EFF) {
        uint16_t mirrored_address;

        if (address >= 0x3000) {
            address -= 0x1000;
        }

        mirrored_address =
            ppu_mirror_nametable_address(ppu, address);

        ppu->nametable_ram[mirrored_address] = value;
        return;
    }

    // Palette memory: $3F00-$3FFF
    if (address >= 0x3F00) {
        uint16_t index =
            ppu_mirror_palette_address(address);

        ppu->palette_ram[index] = value & 0x3F;
    }
}

// Read PPU registers through the CPU bus.
uint8_t ppu_cpu_read(PPU* ppu, uint16_t address) {
    uint16_t reg = address & 0x0007;

    switch (reg) {
        case 0:
            return ppu->control;

        case 1:
            return ppu->mask;

        case 2: {
            uint8_t value = ppu->status;

            // Reading PPUSTATUS clears VBlank
            // and resets the write latch.
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
            uint8_t value;

            // Palette reads are not delayed.
            if (ppu->vram_addr >= 0x3F00) {
                value = ppu_read_memory(
                    ppu,
                    ppu->vram_addr
                );

                // The internal buffer is still updated
                // from the mirrored nametable address.
                ppu->data_buffer = ppu_read_memory(
                    ppu,
                    ppu->vram_addr - 0x1000
                );
            } else {
                value = ppu->data_buffer;

                ppu->data_buffer = ppu_read_memory(
                    ppu,
                    ppu->vram_addr
                );
            }

            ppu_increment_vram_address(ppu);

            return value;
        }

        default:
            return 0;
    }
}

// Write to PPU registers through the CPU bus.
void ppu_cpu_write(
    PPU* ppu,
    uint16_t address,
    uint8_t value
) {
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
            ppu_write_memory(
                ppu,
                ppu->vram_addr,
                value
            );

            ppu_increment_vram_address(ppu);
            break;

        default:
            break;
    }
}
