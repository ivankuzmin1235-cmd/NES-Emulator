#include "6502.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {

    CPU_6502 cpu;
 
    cpu_init(&cpu, "nestest.nes");
    
    while(1){
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
    return 0;
}
