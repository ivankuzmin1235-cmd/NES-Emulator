#include "6502.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {

    CPU_6502 cpu;
    
    cpu_init(&cpu);
    
    cpu_load_rom(&cpu, "nestest.nes");
    

    cpu.pc = 0xC000;
    cpu.sp = 0xFD;
    cpu.p = 0x24;

    for (int i = 0; i < 128; i++)
    {
        printf("%x", cpu.ram[0x8000 + i]);
    }
    
    while(1){
        cpu_step(&cpu);
        
        // Проверяем результаты в $02 и $03
        if(cpu.ram[0x02] != 0 || cpu.ram[0x03] != 0) {
            printf("Test failed! Error code: %02X%02X\n", cpu.ram[0x03], cpu.ram[0x02]);
            break;
        }
        
    }
    
    
      
    return 0;
}
