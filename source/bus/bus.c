#include "bus.h"

#include <stdio.h>
#include <string.h>
#include "../PPU/ppu.h"

void bus_init(Bus* bus) {
    memset(bus, 0, sizeof(Bus));

    ppu_init(&bus->ppu);
}

uint8_t bus_read(Bus* bus, uint16_t address) {

    // Internal RAM: $0000-$1FFF
    if (address <= 0x1FFF) {
        return bus->ram[address & 0x07FF];
    }

    // PPU registers: $2000-$3FFF
    if (address <= 0x3FFF) {
        return ppu_cpu_read(&bus->ppu, address);
    }

    // APU and I/O: $4000-$7FFF
    if (address <= 0x7FFF) {
        return 0;
    }

    // PRG ROM: $8000-$FFFF
    if (address >= 0x8000) {
        size_t offset = address - 0x8000;

        if (bus->prg_rom_size == 16384) {
            offset &= 0x3FFF;
        }

        return bus->prg_rom[offset];
    }

    return 0;
}

void bus_write(Bus* bus, uint16_t address, uint8_t value) {

    // Internal RAM: $0000-$1FFF
    if (address <= 0x1FFF) {
        bus->ram[address & 0x07FF] = value;
        return;
    }

    // PPU registers: $2000-$3FFF
    if (address <= 0x3FFF) {
        ppu_cpu_write(&bus->ppu, address, value);
        return;
    }
}



int bus_load_prg_rom(Bus* bus, const char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("ERROR: Cannot open ROM file %s\n", filename);
        return 0;
    }

    // Read iNES header
    uint8_t header[16];

    if (fread(header, 1, sizeof(header), file) != sizeof(header)) {
        printf("ERROR: Cannot read ROM header\n");
        fclose(file);
        return 0;
    }

    // Check iNES signature
    if (header[0] != 'N' ||
        header[1] != 'E' ||
        header[2] != 'S' ||
        header[3] != 0x1A) {

        printf("ERROR: Invalid NES ROM header\n");
        fclose(file);
        return 0;
    }

    // Reject NES 2.0 format for now
    if ((header[7] & 0x0C) == 0x08) {
        printf("ERROR: NES 2.0 format is not supported yet\n");
        fclose(file);
        return 0;
    }

    // Extract mapper number
    uint8_t mapper = (header[6] >> 4) | (header[7] & 0xF0);

    if (mapper != 0) {
        printf("ERROR: Unsupported Mapper: %u\n", mapper);
        fclose(file);
        return 0;
    }

    // Calculate PRG ROM size
    size_t prg_size = (size_t)header[4] * 16384;

    if (prg_size != 16384 && prg_size != 32768) {
        printf("ERROR: Unsupported PRG ROM size: %zu\n", prg_size);
        fclose(file);
        return 0;
    }

    // Calculate CHR ROM size
    uint8_t chr_banks = header[5];

    if (chr_banks > 1) {
        printf("ERROR: Unsupported CHR ROM size: %u banks\n", chr_banks);
        fclose(file);
        return 0;
    }

    // Skip optional 512-byte Trainer
    if (header[6] & 0x04) {
        if (fseek(file, 512, SEEK_CUR) != 0) {
            printf("ERROR: Cannot skip Trainer\n");
            fclose(file);
            return 0;
        }
    }

    // Read PRG ROM
    if (fread(bus->prg_rom, 1, prg_size, file) != prg_size) {
        printf("ERROR: Cannot read complete PRG ROM\n");
        fclose(file);
        return 0;
    }

    bus->prg_rom_size = prg_size;

    // Read CHR ROM or initialize CHR RAM
    if (chr_banks > 0) {
        if (fread(
            bus->ppu.chr,
            1,
            PPU_CHR_SIZE,
            file
        ) != PPU_CHR_SIZE) {
            printf("ERROR: Cannot read complete CHR ROM\n");
            fclose(file);
            return 0;
        }

        bus->ppu.chr_is_ram = 0;

        printf("Loaded CHR ROM: %u bytes\n", PPU_CHR_SIZE);
    } else {
        memset(bus->ppu.chr, 0, PPU_CHR_SIZE);

        bus->ppu.chr_is_ram = 1;

        printf("Using CHR RAM: %u bytes\n", PPU_CHR_SIZE);
    }

    // Configure nametable mirroring
    if (header[6] & 0x08) {
        bus->ppu.mirroring = MIRRORING_FOUR_SCREEN;
    } else if (header[6] & 0x01) {
        bus->ppu.mirroring = MIRRORING_VERTICAL;
    } else {
        bus->ppu.mirroring = MIRRORING_HORIZONTAL;
    }

    fclose(file);

    printf("Loaded PRG ROM: %zu bytes\n", prg_size);
    printf("Mapper: %u (NROM)\n", mapper);

    return 1;
}
