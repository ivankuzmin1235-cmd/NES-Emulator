#include "bus.h"

#include <stdio.h>
#include <string.h>

void bus_init(Bus* bus) {
    memset(bus, 0, sizeof(Bus));
}

uint8_t bus_read(Bus* bus, uint16_t address) {

    if (address <= 0x1FFF) {
        return bus->ram[address & 0x07FF];
    }

    if (address <= 0x7FFF) {
        return 0;
    }

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

    if (address <= 0x1FFF) {
        bus->ram[address & 0x07FF] = value;
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

    fclose(file);

    printf("Loaded PRG ROM: %zu bytes\n", prg_size);
    printf("Mapper: %u (NROM)\n", mapper);

    return 1;
}
