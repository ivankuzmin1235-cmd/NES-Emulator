#include <stdio.h>

int main() {
    printf("HELLO DENIS");
    return 0;
}
=======
#include "6502.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    CPU_6502 cpu;
    cpu_init(&cpu);
    
    for (int i = 0; i < 32767; i++)
    {
        printf("%02X ", cpu.ram[0x8000 + i]);
        sleep(0.1); 
    }
    
    
    /*
    while(1){
        cpu_step(&cpu); 
        sleep(1); 
    }*/
    
    
      
    return 0;
}
