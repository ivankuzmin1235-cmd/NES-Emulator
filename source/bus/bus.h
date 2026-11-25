#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stddef.h>
#include "../PPU/ppu.h"

#define RAM_SIZE 2048
#define PRG_ROM_MAX_SIZE 32768

typedef struct {
    uint8_t ram[RAM_SIZE];

    uint8_t prg_rom[PRG_ROM_MAX_SIZE];
    size_t prg_rom_size;

    PPU ppu;

} Bus;

void bus_init(Bus* bus);

uint8_t bus_read(Bus* bus, uint16_t address);

void bus_write(Bus* bus, uint16_t address, uint8_t value);

int bus_load_prg_rom(Bus* bus, const char* filename);

#endif