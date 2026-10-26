#include "6502.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {

    CPU_6502 cpu;
 
    cpu_init(&cpu, "nestest.nes");

    printf("Initial PC = $%04X\n", cpu.pc);
    
    for (unsigned long i = 0; i < 1000000; i++) {
        if(i < 20) {
            printf("Step %lu: PC=$%04X OPCODE=$%02X A=$%02X X=$%02X P=$%02X SP=$%02X\n", i, cpu.pc, cpu_read(&cpu, cpu.pc), cpu.a, cpu.x, cpu.y, cpu.p, cpu.sp);
        }
        cpu_step(&cpu);
        if(cpu.ram[0x02] != 0 || cpu.ram[0x03] != 0) {
            uint16_t error = (cpu.ram[0x03] << 8) | cpu.ram[0x02];
            if(error == 0) {
                printf("✅ ALL TESTS PASSED!\n");
            } else {
                printf("❌ TEST FAILED! Error code: %04X\n", error);
            }
            break;
        }
    }

    printf("Execution loop endeed.\n");
    printf("PC = $%04X\n", cpu.pc);
    printf("A = $%02X\n X = $%02X, Y = $%02X\n", cpu.a, cpu.x, cpu.y);
    return 0;
}
