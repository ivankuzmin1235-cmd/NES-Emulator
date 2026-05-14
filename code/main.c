#include "6502.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {

    CPU_6502 cpu;
    
    cpu_init(&cpu);
    
    cpu_load_bin(&cpu, "cpu_test.bin");
    
    while(1){
        if(cpu_step(&cpu) == 1){
            for (int i = 0; i < (0x6024 - 0x6000); i++)
            {
                printf("%x\n",cpu.ram[0x6000 + i]);
            }
            break;
        } 
        
    }
    
    
      
    return 0;
}