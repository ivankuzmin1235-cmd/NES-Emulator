#include <stdint.h>
#include "stdio.h"

#define ROM_SPACE (0xFFFF - 0x8000)
// Флаги процессора 6502 (битовые позиции)
#define FLAG_CARRY    0x01  // Бит 0: перенос
#define FLAG_ZERO     0x02  // Бит 1: ноль
#define FLAG_IRQ_DIS  0x04  // Бит 2: запрет прерываний
#define FLAG_DECIMAL  0x08  // Бит 3: десятичный режим (не используется в NES)
#define FLAG_BREAK    0x10  // Бит 4: программное прерывание
#define FLAG_OVERFLOW 0x40  // Бит 6: переполнение
#define FLAG_NEGATIVE 0x80  // Бит 7: отрицательное число

typedef struct {
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t sp;
    uint8_t p;
    uint16_t pc;
    uint64_t cycles;
    uint8_t ram[0x10000]; /*64KB RAM*/
  
}CPU_6502;

typedef struct {
        uint8_t opcode;
        void (*execute)(CPU_6502*);

}Instruction;

Instruction opcode_list[0xFF];

void opcodes_init(CPU_6502* cpu){
                /*LDA OPCODES*/
    opcode_list[0xA9].execute = lda_immediate;
    opcode_list[0xAD].execute = lda_absolute;
    opcode_list[0xBD].execute = lda_absolute_x;
    opcode_list[0xB9].execute = lda_absolute_y;
    opcode_list[0xA5].execute = lda_zeropage;
    opcode_list[0xB5].execute = lda_zeropage_x;
    opcode_list[0xA1].execute = lda_indirect_x;
    opcode_list[0xB1].execute = lda_indirect_y;
                /*LDX OPCODES*/
    opcode_list[0xA2].execute = ldx_immediate;
    opcode_list[0xAE].execute = ldx_absolute;
    opcode_list[0xBE].execute = ldx_absolute_y;
    opcode_list[0xA6].execute = ldx_zeropage;
    opcode_list[0xB6].execute = ldx_zeropage_y;
                /*LDY OPCODES*/
    opcode_list[0xA0].execute = ldy_immediate;
    opcode_list[0xAC].execute = ldy_absolute;
    opcode_list[0xBC].execute = ldy_absolute_y;
    opcode_list[0xA4].execute = ldy_zeropage;
    opcode_list[0xB4].execute = ldy_zeropage_y;
                /*STA OPCODES*/
    opcode_list[0x8D].execute = sta_absolute;
    opcode_list[0x9D].execute = sta_absolute_x;
    opcode_list[0x99].execute = sta_absolute_y;
    opcode_list[0x85].execute = sta_zeropage;
    opcode_list[0x95].execute = sta_zeropage_x;
    opcode_list[0x81].execute = sta_indirect_x;
    opcode_list[0x91].execute = sta_indirect_y;
                /*STX OPCODES*/
    opcode_list[0x8E].execute = stx_absolute;
    opcode_list[0x86].execute = stx_zeropage;
    opcode_list[0x96].execute = stx_zeropage_y;
                /*STY OPCODES*/
    opcode_list[0x8C].execute = sty_absolute;
    opcode_list[0x84].execute = sty_zeropage;
    opcode_list[0x94].execute = sty_zeropage_x;
        /*TAX,TAY,TSX,TXA,TXS,TYA OPCODES*/
    opcode_list[0xAA].execute = tax_implied;
    opcode_list[0xA8].execute = tay_implied;
    opcode_list[0xBA].execute = tsx_implied;
    opcode_list[0x8A].execute = txa_implied;
    opcode_list[0x9A].execute = txs_implied;
    opcode_list[0x98].execute = tya_implied;
        /*PHA, PHP, PLA, PLP OPCODES*/
    opcode_list[0x48].execute = pha_implied;
    opcode_list[0x08].execute = php_implied;
    opcode_list[0x68].execute = pla_implied;
    opcode_list[0x28].execute = plp_implied;
    
                /*SHIFT HERE*/
    
    
    
    
    
                /*AND OPCODES*/
    opcode_list[0x29].execute = and_immediate;
    opcode_list[0x2D].execute = and_absolute;
    opcode_list[0x3D].execute = and_absolute_x;
    opcode_list[0x39].execute = and_absolute_y;
    opcode_list[0x25].execute = and_zeropage;
    opcode_list[0x35].execute = and_zeropage_x;
    opcode_list[0x21].execute = and_indirect_x;
    opcode_list[0x31].execute = and_indirect_y;
                /*BIT OPCODES*/
    opcode_list[0x2C].execute = bit_absolute;
    opcode_list[0x24].execute = bit_zeropage; 
                /*EOR OPCODES*/ 
    opcode_list[0x49].execute = eor_immediate;
    opcode_list[0x4D].execute = eor_absolute;
    opcode_list[0x5D].execute = eor_absolute_x;
    opcode_list[0x59].execute = eor_absolute_y;
    opcode_list[0x45].execute = eor_zeropage;
    opcode_list[0x55].execute = eor_zeropage_x;
    opcode_list[0x41].execute = eor_indirect_x;
    opcode_list[0x51].execute = eor_indirect_y; 
                /*ORA OPCODES*/ 
    opcode_list[0x09].execute = ora_immediate;
    opcode_list[0x0D].execute = ora_absolute;
    opcode_list[0x1D].execute = ora_absolute_x;
    opcode_list[0x19].execute = ora_absolute_y;
    opcode_list[0x05].execute = ora_zeropage;
    opcode_list[0x15].execute = ora_zeropage_x;
    opcode_list[0x01].execute = ora_indirect_x;
    opcode_list[0x11].execute = ora_indirect_y; 
                /*ADC OPCODES*/
    opcode_list[0x69].execute = adc_immediate;
    opcode_list[0x65].execute = adc_zeropage;
    opcode_list[0x75].execute = adc_zeropage_x;
    opcode_list[0x6D].execute = adc_absolute;
    opcode_list[0x7D].execute = adc_absolute_x;
    opcode_list[0x79].execute = adc_absolute_y;
    opcode_list[0x61].execute = adc_indirect_x;
    opcode_list[0x71].execute = adc_indirect_y;
                /*CMP OPCODES*/
    opcode_list[0xC9].execute = cmp_immediate;
    opcode_list[0xC5].execute = cmp_zeropage;
    opcode_list[0xD5].execute = cmp_zeropage_x;
    opcode_list[0xCD].execute = cmp_absolute;
    opcode_list[0xDD].execute = cmp_absolute_x;
    opcode_list[0xD9].execute = cmp_absolute_y;
    opcode_list[0xC1].execute = cmp_indirect_x;
    opcode_list[0xD1].execute = cmp_indirect_y;
                /*CPX OPCODES*/
    opcode_list[0xE0].execute = cpx_immediate;
    opcode_list[0xE4].execute = cpx_zeropage;
    opcode_list[0xEC].execute = cpx_absolute;
                /*CPY OPCODES*/
    opcode_list[0xC0].execute = cpy_immediate;
    opcode_list[0xC4].execute = cpy_zeropage;
    opcode_list[0xCC].execute = cpy_absolute;
                /*SBC OPCODES*/
    opcode_list[0xE9].execute = sbc_immediate;
    opcode_list[0xE5].execute = sbc_zeropage;
    opcode_list[0xF5].execute = sbc_zeropage_x;
    opcode_list[0xED].execute = sbc_absolute;
    opcode_list[0xFD].execute = sbc_absolute_x;
    opcode_list[0xF9].execute = sbc_absolute_y;
    opcode_list[0xE1].execute = sbc_indirect_x;
    opcode_list[0xF1].execute = sbc_indirect_y;
                /*DEC OPCODES*/   
    opcode_list[0xCE].execute = dec_absolute;
    opcode_list[0xDE].execute = dec_absolute_x;
    opcode_list[0xC6].execute = dec_zeropage;
    opcode_list[0xD6].execute = dec_zeropage_x;
                /*DEX, DEY OPCODES*/ 
    opcode_list[0xCA].execute = dex_implied;
    opcode_list[0x88].execute = dey_implied; 
                /*INC OPCODES*/
    opcode_list[0xEE].execute = inc_absolute;
    opcode_list[0xFE].execute = inc_absolute_x;
    opcode_list[0xE6].execute = inc_zeropage;
    opcode_list[0xF6].execute = inc_zeropage_x;
                /*INX, INY OPCODES*/ 
    opcode_list[0xE8].execute = inx_implied;
    opcode_list[0xC8].execute = iny_implied;     
                /*FLAGS OPCODES*/
    opcode_list[0x18].execute = clc_implied;
    opcode_list[0x38].execute = sec_implied;
    opcode_list[0xD8].execute = cld_implied;
    opcode_list[0xF8].execute = sed_implied;
    opcode_list[0x58].execute = cli_implied;
    opcode_list[0x78].execute = sei_implied;
    opcode_list[0xB8].execute = clv_implied;
}
  
                                                            /*CPU STUFF*/
void cpu_load_rom(CPU_6502* cpu, const char* filename){
    FILE* f = fopen(filename, "rb");
    if (!f) {
        printf("Не удалось открыть файл: %s\n", filename);
        return;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if (size > ROM_SPACE) {
        printf("ROM is way TOO BIG");
        fclose(f);
        
        
    }
    fread(cpu->ram + 0x8000, 1, size, f);
    
    fclose(f);

    
}

void cpu_init(CPU_6502* cpu){
    cpu->cycles = 0;
    cpu->sp = cpu->ram[0xFD];
    cpu->pc = 0x8000;
    cpu_load_rom(cpu, "");
    opcodes_init(cpu);
}

uint8_t cpu_read(CPU_6502* cpu, uint16_t address){
    return cpu->ram[address];
}
void cpu_write(CPU_6502* cpu, uint16_t address, uint8_t value){
    cpu->ram[address] = value;
}

                                                          /*ADDRESS STUFF*/
/*This part contains different functions for addressing modes of NES 6502, we will use it to count address of operand for instructions*/
uint16_t get_absolute_addr(CPU_6502* cpu){
    uint8_t m_addr_lo = cpu_read(cpu, cpu->pc++);/*firstly lo byte Example: 0x34*/
    uint8_t m_addr_hi = cpu_read(cpu, cpu->pc++);/*Then hi byte Example: 0x12*/
    uint16_t m_addr = (m_addr_hi << 8) | m_addr_lo;/*0x1234*/

    return m_addr;
}
uint16_t get_absolute_x_addr(CPU_6502* cpu, uint8_t* extra_cycle){
    uint8_t m_addr_lo = cpu_read(cpu, cpu->pc++);
    uint8_t m_addr_hi = cpu_read(cpu, cpu->pc++);
    uint16_t m_base_addr = (m_addr_hi << 8) | m_addr_lo;
    uint16_t m_addr = m_base_addr + cpu->x;
    
    *extra_cycle = ((m_base_addr & 0xFF00) != (m_addr & 0xFF00)) ? 1 : 0;/*Проверка перехода*/
    

    return m_addr;
}
uint16_t get_absolute_y_addr(CPU_6502* cpu, uint8_t* extra_cycle){
    uint8_t m_addr_lo = cpu_read(cpu, cpu->pc++);
    uint8_t m_addr_hi = cpu_read(cpu, cpu->pc++);
    uint16_t m_base_addr = (m_addr_hi << 8) | m_addr_lo;
    uint16_t m_addr = m_base_addr + cpu->y;

    *extra_cycle = ((m_base_addr & 0xFF00) != (m_addr & 0xFF00)) ? 1 : 0;

    return m_addr;

}
uint16_t get_zeropage_addr(CPU_6502* cpu){
    uint16_t m_addr = cpu_read(cpu, cpu->pc++);

    return m_addr;
}
uint16_t get_zeropage_x_addr(CPU_6502* cpu){
    uint16_t m_addr = (cpu_read(cpu, cpu->pc++) + cpu->x) & 0xFF;

    return m_addr;
}
uint16_t get_zeropage_y_addr(CPU_6502* cpu){
    uint16_t m_addr = (cpu_read(cpu, cpu->pc++) + cpu->y) & 0xFF;

    return m_addr;
}
uint16_t get_indirect_x_addr(CPU_6502* cpu){
    uint8_t zp_addr = cpu_read(cpu, cpu->pc++) + cpu->x;
    
    
    uint16_t lo = cpu_read(cpu, zp_addr);
    uint16_t hi = cpu_read(cpu, (zp_addr + 1) & 0xFF);
    uint16_t m_addr = (hi << 8) | lo;

    return m_addr;
}
uint16_t get_indirect_y_addr(CPU_6502* cpu, uint8_t* extra_cycle){
    uint8_t zp_addr = cpu_read(cpu, cpu->pc++);
    
    
    uint16_t m_addr_lo = cpu_read(cpu, zp_addr);
    uint16_t m_addr_hi = cpu_read(cpu, (zp_addr + 1) & 0xFF);
    uint16_t m_base_addr = (m_addr_hi << 8) | m_addr_lo;
    uint16_t m_addr = m_base_addr + cpu->y;

    *extra_cycle = ((m_base_addr & 0xFF00) != (m_addr & 0xFF00)) ? 1 : 0;

    return m_addr;
}
                                                            /*LDA STUFF */
void lda_flags_do(CPU_6502* cpu){
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;/*флаг в 1*/
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->a & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void lda_immediate(CPU_6502* cpu){ 
    uint8_t m = cpu_read(cpu, cpu->pc++);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 2;
}
void lda_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);/*Адрес*/
    uint8_t m = cpu_read(cpu, m_addr);/*Читаем*/
    cpu->a = m;/*Кладём*/
    lda_flags_do(cpu);/*Делаем флаги*/
    cpu->cycles += 4;/*увеличиваем кол-во циклов*/
}
void lda_absolute_x(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_x_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void lda_absolute_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void lda_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 3;
}
void lda_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 4;
}
void lda_indirect_x(CPU_6502* cpu){
    uint16_t m_addr = get_indirect_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 6;
}
void lda_indirect_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_indirect_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m;
    lda_flags_do(cpu);
    cpu->cycles += 5 + extra;
}
                                                            /*LDX STUFF*/
void ldx_flags_do(CPU_6502* cpu){
    if(cpu->x == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
       cpu->p &= ~FLAG_ZERO; 
    }
    if((cpu->x & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void ldx_immediate(CPU_6502* cpu){ 
    uint8_t m = cpu_read(cpu, cpu->pc++);
    cpu->x = m;
    ldx_flags_do(cpu);
    cpu->cycles += 2;
}
void ldx_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);/*Адрес*/
    uint8_t m = cpu_read(cpu, m_addr);/*Читаем*/
    cpu->x = m;/*Кладём*/
    ldx_flags_do(cpu);/*Делаем флаги*/
    cpu->cycles += 4;/*увеличиваем кол-во циклов*/
}
void ldx_absolute_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->x = m;
    ldx_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void ldx_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->x = m;
    ldx_flags_do(cpu);
    cpu->cycles += 3;
}
void ldx_zeropage_y(CPU_6502* cpu){

    uint16_t m_addr = get_zeropage_y_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->x = m;
    ldx_flags_do(cpu);
    cpu->cycles += 4;
}
                                                            /*LDY STUFF*/
void ldy_flags_do(CPU_6502* cpu){
    if(cpu->y == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
       cpu->p &= ~FLAG_ZERO; 
    }
    if((cpu->y & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void ldy_immediate(CPU_6502* cpu){ 
    uint8_t m = cpu_read(cpu, cpu->pc++);
    cpu->y = m;
    ldy_flags_do(cpu);
    cpu->cycles += 2;
}
void ldy_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);/*Адрес*/
    uint8_t m = cpu_read(cpu, m_addr);/*Читаем*/
    cpu->y = m;/*Кладём*/
    ldy_flags_do(cpu);/*Делаем флаги*/
    cpu->cycles += 4;/*увеличиваем кол-во циклов*/
}
void ldy_absolute_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->y = m;
    ldy_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void ldy_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->y = m;
    ldy_flags_do(cpu);
    cpu->cycles += 3;
}
void ldy_zeropage_y(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_y_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->y = m;
    ldy_flags_do(cpu);
    cpu->cycles += 4;
}
                                                            /*STA STUFF*/
void sta_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 4;
}
void sta_absolute_x(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_x_addr(cpu, NULL);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 5;
}
void sta_absolute_y(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_y_addr(cpu, NULL);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 5;
}
void sta_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 3;
}
void sta_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 4;
}
void sta_indirect_x(CPU_6502* cpu){
    uint16_t m_addr = get_indirect_x_addr(cpu);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 6;
}
void sta_indirect_y(CPU_6502* cpu){
    uint16_t m_addr = get_indirect_y_addr(cpu, NULL);
    cpu_write(cpu, m_addr, cpu->a);

    cpu->cycles += 6;
}
                                                            /*STX STUFF*/
void stx_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    cpu_write(cpu, m_addr, cpu->x);

    cpu->cycles += 4;
}
void stx_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    cpu_write(cpu, m_addr, cpu->x);

    cpu->cycles += 3;
}
void stx_zeropage_y(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_y_addr(cpu);
    cpu_write(cpu, m_addr, cpu->x);

    cpu->cycles += 4;
}
                                                            /*STY STUFF*/
void sty_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    cpu_write(cpu, m_addr, cpu->y);

    cpu->cycles += 4;
}
void sty_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    cpu_write(cpu, m_addr, cpu->y);

    cpu->cycles += 3;
}
void sty_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    cpu_write(cpu, m_addr, cpu->y);

    cpu->cycles += 4;
}
                                                            /*TAX STUFF*/
void tax_flags_do(CPU_6502* cpu){
    if(cpu->x == 0){
        cpu->p |= FLAG_ZERO;/*флаг в 1*/
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->x & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}                                                            
void tax_implied(CPU_6502* cpu){
    cpu->x = cpu->a;
    tax_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*TAY STUFF*/
void tay_flags_do(CPU_6502* cpu){
    if(cpu->y == 0){
        cpu->p |= FLAG_ZERO;/*флаг в 1*/
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->y & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}                                                            
void tay_implied(CPU_6502* cpu){
    cpu->y = cpu->a;
    tay_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*TSX STUFF*/
void tsx_flags_do(CPU_6502* cpu){
    if(cpu->x == 0){
        cpu->p |= FLAG_ZERO;/*флаг в 1*/
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->x & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}                                                            
void tsx_implied(CPU_6502* cpu){
    cpu->x = cpu->sp;
    tsx_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*TXA STUFF*/
void txa_flags_do(CPU_6502* cpu){
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;/*флаг в 1*/
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->a & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}                                                            
void txa_implied(CPU_6502* cpu){
    cpu->a = cpu->x;
    txa_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*TXS STUFF*/                                                       
void txs_implied(CPU_6502* cpu){
    cpu->sp = cpu->x;
    cpu->cycles += 2;
}
                                                            /*TYA STUFF*/
void tya_flags_do(CPU_6502* cpu){
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;/*флаг в 1*/
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->a & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}                                                            
void tya_implied(CPU_6502* cpu){
    cpu->a = cpu->y;
    tya_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*PHA STUFF*/
void pha_implied(CPU_6502* cpu){
   cpu_write(cpu, 0x0100 + cpu->sp, cpu->a);
   cpu->sp--;

   cpu->cycles+=3;
}
                                                            /*PHP STUFF*/
void php_implied(CPU_6502* cpu){
    cpu_write(cpu, 0x0100 + cpu->sp, cpu->p);

    cpu->cycles+=3;
}
                                                            /*PLA STUFF*/
void pla_flags_do(CPU_6502* cpu){
    if((cpu->a >> 7) & 1){
       cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
       cpu->p &= ~FLAG_ZERO;
    }
}
void pla_implied(CPU_6502* cpu){
    cpu->a = cpu_read(cpu, 0x0100 + cpu->sp+1);
    cpu->sp++;
    pla_flags_do(cpu);

    cpu->cycles += 4;
}
                                                            /*PLP STUFF*/
void plp_implied(CPU_6502* cpu){
    cpu->p = (cpu_read(cpu, 0x0100 + cpu->sp+1)) | 0x20;
    cpu->sp++;
    cpu->cycles += 4;
}
                                                            /*AND STUFF*/
void and_flags_do(CPU_6502* cpu){
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->a & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void and_immediate(CPU_6502* cpu){
    uint8_t m = cpu_read(cpu, cpu->pc++);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 2;
}
void and_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 4;
}
void and_absolute_x(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_x_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void and_absolute_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void and_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 3;
}
void and_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 4;
}
void and_indirect_x(CPU_6502* cpu){
    uint16_t m_addr = get_indirect_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 6;
}
void and_indirect_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_indirect_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m & cpu->a;
    and_flags_do(cpu);
    cpu->cycles += 5 + extra;
}
                                                            /*BIT STUFF*/
void bit_flags_do(CPU_6502* cpu, uint8_t m, uint8_t bband){
    if(bband == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    cpu->p = (cpu->p & ~FLAG_NEGATIVE) | (m & FLAG_NEGATIVE);
    cpu->p = (cpu->p & ~FLAG_OVERFLOW) | (m & FLAG_OVERFLOW);
}                                                           
void bit_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);

    uint8_t bband = cpu->a & m;
    
    bit_flags_do(cpu, m, bband);

    cpu->cycles += 4;
}
void bit_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);

    uint8_t bband = cpu->a & m;
    
    bit_flags_do(cpu, m, bband);

    cpu->cycles += 3;
}
                                                            /*EOR STUFF*/
void eor_flags_do(CPU_6502* cpu){
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->a & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void eor_immediate(CPU_6502* cpu){
    uint8_t m = cpu_read(cpu, cpu->pc++);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 2;
}
void eor_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 4;
}
void eor_absolute_x(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_x_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void eor_absolute_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void eor_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 3;
}
void eor_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 4;
}
void eor_indirect_x(CPU_6502* cpu){
    uint16_t m_addr = get_indirect_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 6;
}
void eor_indirect_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_indirect_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m ^ cpu->a;
    eor_flags_do(cpu);
    cpu->cycles += 5 + extra;
}
                                                            /*ORA STUFF*/
void ora_flags_do(CPU_6502* cpu){
    if(cpu->a == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->a & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void ora_immediate(CPU_6502* cpu){
    uint8_t m = cpu_read(cpu, cpu->pc++);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 2;
}
void ora_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 4;
}
void ora_absolute_x(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_x_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void ora_absolute_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_absolute_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 4 + extra;
}
void ora_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 3;
}
void ora_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 4;
}
void ora_indirect_x(CPU_6502* cpu){
    uint16_t m_addr = get_indirect_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 6;
}
void ora_indirect_y(CPU_6502* cpu){
    uint8_t extra;
    uint16_t m_addr = get_indirect_y_addr(cpu, &extra);
    uint8_t m = cpu_read(cpu, m_addr);
    cpu->a = m | cpu->a;
    ora_flags_do(cpu);
    cpu->cycles += 5 + extra;
}
                                                            /*ADC STUFF*/
void adc_flags_and_store(CPU_6502* cpu, uint8_t operand) {
    uint8_t carry_in = (cpu->p & FLAG_CARRY) ? 1 : 0;
    uint16_t sum = cpu->a + operand + carry_in;
    
    if (((cpu->a ^ operand) & 0x80) == 0 && ((cpu->a ^ sum) & 0x80) != 0)
        cpu->p |= FLAG_OVERFLOW;
    else
        cpu->p &= ~FLAG_OVERFLOW;
    
    
    if (sum > 0xFF)
        cpu->p |= FLAG_CARRY;
    else
        cpu->p &= ~FLAG_CARRY;
    
    uint8_t result = sum & 0xFF;
    cpu->a = result;
    
    if (result == 0)
        cpu->p |= FLAG_ZERO;
    else
        cpu->p &= ~FLAG_ZERO;
    
    if (result & 0x80)
        cpu->p |= FLAG_NEGATIVE;
    else
        cpu->p &= ~FLAG_NEGATIVE;
}
void adc_immediate(CPU_6502* cpu) {
    uint8_t operand = cpu_read(cpu, cpu->pc++);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 2;
}
void adc_zeropage(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 3;
}
void adc_zeropage_x(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_x_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 4;
}
void adc_absolute(CPU_6502* cpu) {
    uint16_t addr = get_absolute_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 4;
}
void adc_absolute_x(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_absolute_x_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 4 + extra;
}
void adc_absolute_y(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_absolute_y_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 4 + extra;
}
void adc_indirect_x(CPU_6502* cpu) {
    uint16_t addr = get_indirect_x_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 6;
}
void adc_indirect_y(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_indirect_y_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    adc_flags_and_store(cpu, operand);
    cpu->cycles += 5 + extra;
}
                                                            /*CMP STUFF*/
void cmp_flags_do(CPU_6502* cpu, uint8_t operand) {
    uint8_t result = cpu->a - operand;
    
    if (cpu->a >= operand)
        cpu->p |= FLAG_CARRY;
    else
        cpu->p &= ~FLAG_CARRY;
    
    
    if (result == 0)
        cpu->p |= FLAG_ZERO;
    else
        cpu->p &= ~FLAG_ZERO;
    
    
    if (result & 0x80)
        cpu->p |= FLAG_NEGATIVE;
    else
        cpu->p &= ~FLAG_NEGATIVE;
}
void cmp_immediate(CPU_6502* cpu) {
    uint8_t operand = cpu_read(cpu, cpu->pc++);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 2;
}
void cmp_zeropage(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 3;
}
void cmp_zeropage_x(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_x_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 4;
}
void cmp_absolute(CPU_6502* cpu) {
    uint16_t addr = get_absolute_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 4;
}
void cmp_absolute_x(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_absolute_x_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 4 + extra;
}
void cmp_absolute_y(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_absolute_y_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 4 + extra;
}
void cmp_indirect_x(CPU_6502* cpu) {
    uint16_t addr = get_indirect_x_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 6;
}
void cmp_indirect_y(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_indirect_y_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    cmp_flags_do(cpu, operand);
    cpu->cycles += 5 + extra;
}
                                                            /*CPX STUFF*/
void cpx_flags_do(CPU_6502* cpu, uint8_t operand) {
    uint8_t result = cpu->x - operand;
    
    if (cpu->x >= operand)
        cpu->p |= FLAG_CARRY;
    else
        cpu->p &= ~FLAG_CARRY;
    
    if (result == 0)
        cpu->p |= FLAG_ZERO;
    else
        cpu->p &= ~FLAG_ZERO;
    
    if (result & 0x80)
        cpu->p |= FLAG_NEGATIVE;
    else
        cpu->p &= ~FLAG_NEGATIVE;
}
void cpx_immediate(CPU_6502* cpu) {
    uint8_t operand = cpu_read(cpu, cpu->pc++);
    cpx_flags_do(cpu, operand);
    cpu->cycles += 2;
}
void cpx_zeropage(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cpx_flags_do(cpu, operand);
    cpu->cycles += 3;
}
void cpx_absolute(CPU_6502* cpu) {
    uint16_t addr = get_absolute_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cpx_flags_do(cpu, operand);
    cpu->cycles += 4;
}
                                                            /*CPY STUFF*/
void cpy_flags_do(CPU_6502* cpu, uint8_t operand) {
    uint8_t result = cpu->y - operand;
    
    if (cpu->y >= operand)
        cpu->p |= FLAG_CARRY;
    else
        cpu->p &= ~FLAG_CARRY;
    
    if (result == 0)
        cpu->p |= FLAG_ZERO;
    else
        cpu->p &= ~FLAG_ZERO;
    
    if (result & 0x80)
        cpu->p |= FLAG_NEGATIVE;
    else
        cpu->p &= ~FLAG_NEGATIVE;
}
void cpy_immediate(CPU_6502* cpu) {
    uint8_t operand = cpu_read(cpu, cpu->pc++);
    cpy_flags_do(cpu, operand);
    cpu->cycles += 2;
}
void cpy_zeropage(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cpy_flags_do(cpu, operand);
    cpu->cycles += 3;
}
void cpy_absolute(CPU_6502* cpu) {
    uint16_t addr = get_absolute_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    cpy_flags_do(cpu, operand);
    cpu->cycles += 4;
}
                                                            /*SBC STUFF*/
void sbc_flags_and_store(CPU_6502* cpu, uint8_t operand) {
    uint8_t inverted = ~operand;
    uint8_t carry_in = (cpu->p & FLAG_CARRY) ? 1 : 0;
    uint16_t sum = cpu->a + inverted + carry_in;
    
    if (((cpu->a ^ inverted) & 0x80) == 0 && ((cpu->a ^ sum) & 0x80) != 0)
        cpu->p |= FLAG_OVERFLOW;
    else
        cpu->p &= ~FLAG_OVERFLOW;
    
    if (sum > 0xFF)
        cpu->p |= FLAG_CARRY;
    else
        cpu->p &= ~FLAG_CARRY;
    
    uint8_t result = sum & 0xFF;
    cpu->a = result;
    
    if (result == 0)
        cpu->p |= FLAG_ZERO;
    else
        cpu->p &= ~FLAG_ZERO;
    
    if (result & 0x80)
        cpu->p |= FLAG_NEGATIVE;
    else
        cpu->p &= ~FLAG_NEGATIVE;
}
void sbc_immediate(CPU_6502* cpu) {
    uint8_t operand = cpu_read(cpu, cpu->pc++);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 2;
}
void sbc_zeropage(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 3;
}
void sbc_zeropage_x(CPU_6502* cpu) {
    uint16_t addr = get_zeropage_x_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 4;
}
void sbc_absolute(CPU_6502* cpu) {
    uint16_t addr = get_absolute_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 4;
}
void sbc_absolute_x(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_absolute_x_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 4 + extra;
}
void sbc_absolute_y(CPU_6502* cpu) {
    uint8_t extra;
    uint16_t addr = get_absolute_y_addr(cpu, &extra);
    uint8_t operand = cpu_read(cpu, addr);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 4 + extra;
}
void sbc_indirect_x(CPU_6502* cpu) {
    uint16_t addr = get_indirect_x_addr(cpu);
    uint8_t operand = cpu_read(cpu, addr);
    sbc_flags_and_store(cpu, operand);
    cpu->cycles += 6;
}
void sbc_indirect_y(CPU_6502* cpu) {
                                                            /*DEC STUFF*/
void dec_flags_do(CPU_6502* cpu, uint8_t m){
    if(m == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((m & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void dec_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr) - 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 6;
}
void dec_absolute_x(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_x_addr(cpu, NULL);
    uint8_t m = cpu_read(cpu, m_addr) - 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 7;
}
void dec_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr) - 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 5;
}
void dec_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr) - 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 6;
}
                                                            /*DEX STUFF*/
void dex_flags_do(CPU_6502* cpu){
    if(cpu->x == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->x & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void dex_implied(CPU_6502* cpu){
    cpu->x = cpu->x - 1;

    dex_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*DEY STUFF*/
void dey_flags_do(CPU_6502* cpu){
    if(cpu->y == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->y & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void dey_implied(CPU_6502* cpu){

    cpu->y = cpu->y - 1;

    dex_flags_do(cpu);
    cpu->cycles += 2;
}

                                                            /*INC STUFF*/
void inc_flags_do(CPU_6502* cpu, uint8_t m){
    if(m == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((m & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void inc_absolute(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr) + 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 6;
}
void inc_absolute_x(CPU_6502* cpu){
    uint16_t m_addr = get_absolute_x_addr(cpu, NULL);
    uint8_t m = cpu_read(cpu, m_addr) + 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 7;
}
void inc_zeropage(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr) -+ 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 5;
}
void inc_zeropage_x(CPU_6502* cpu){
    uint16_t m_addr = get_zeropage_x_addr(cpu);
    uint8_t m = cpu_read(cpu, m_addr) + 1;
    cpu_write(cpu, m_addr, m);
    dec_flags_do(cpu, m);

    cpu->cycles += 6;
}
                                                            /*INX STUFF*/
void inx_flags_do(CPU_6502* cpu){
    if(cpu->x == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->x & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void inx_implied(CPU_6502* cpu){
    cpu->x = cpu->x + 1;

    dex_flags_do(cpu);
    cpu->cycles += 2;
}
                                                            /*INY STUFF*/
void iny_flags_do(CPU_6502* cpu){
    if(cpu->y == 0){
        cpu->p |= FLAG_ZERO;
    }
    else{
        cpu->p &= ~FLAG_ZERO;
    }
    if((cpu->y & 0x80)){
        cpu->p |= FLAG_NEGATIVE;
    }
    else{
        cpu->p &= ~FLAG_NEGATIVE;
    }
}
void iny_implied(CPU_6502* cpu){
    cpu->y = cpu->y + 1;

    dex_flags_do(cpu);
    cpu->cycles += 2;
}


void cpu_step(CPU_6502* cpu){
    uint8_t opcode = cpu_read(cpu, cpu->pc);
    cpu->pc++;
    opcode_list[opcode].execute(cpu);

}
                                                            /*FLAGS STUFF*/
void clc_implied(CPU_6502* cpu) {
    cpu->p &= ~FLAG_CARRY;
    cpu->cycles += 2;
}
void sec_implied(CPU_6502* cpu) {
    cpu->p |= FLAG_CARRY;
    cpu->cycles += 2;
}
void cld_implied(CPU_6502* cpu) {
    cpu->p &= ~FLAG_DECIMAL;
    cpu->cycles += 2;
}
void sed_implied(CPU_6502* cpu) {
    cpu->p |= FLAG_DECIMAL;
    cpu->cycles += 2;
}
void cli_implied(CPU_6502* cpu) {
    cpu->p &= ~FLAG_IRQ_DIS;
    cpu->cycles += 2;
}
void sei_implied(CPU_6502* cpu) {
    cpu->p |= FLAG_IRQ_DIS;
    cpu->cycles += 2;
}
void clv_implied(CPU_6502* cpu) {
    cpu->p &= ~FLAG_OVERFLOW;
    cpu->cycles += 2;
}
