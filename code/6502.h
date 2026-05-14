#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Flags
#define FLAG_CARRY     0x01
#define FLAG_ZERO      0x02
#define FLAG_IRQ_DIS   0x04
#define FLAG_DECIMAL   0x08
#define FLAG_BREAK     0x10
#define FLAG_OVERFLOW  0x40
#define FLAG_NEGATIVE  0x80

// СPU Structure
typedef struct {
    uint8_t a, x, y, sp, p;
    uint16_t pc;
    uint8_t ram[65536];
    uint64_t cycles;
} CPU_6502;

typedef struct {
        uint8_t opcode;
        void (*execute)(CPU_6502*);

}Instruction;

void opcodes_init(CPU_6502* cpu);
void cpu_load_rom(CPU_6502* cpu, const char* filename);
void cpu_init(CPU_6502* cpu);
uint8_t cpu_read(CPU_6502* cpu, uint16_t address);
void cpu_write(CPU_6502* cpu, uint16_t address, uint8_t value);
uint16_t get_absolute_addr(CPU_6502* cpu);
uint16_t get_absolute_x_addr(CPU_6502* cpu, uint8_t* extra_cycle);
uint16_t get_absolute_y_addr(CPU_6502* cpu, uint8_t* extra_cycle);
uint16_t get_zeropage_addr(CPU_6502* cpu);
uint16_t get_zeropage_x_addr(CPU_6502* cpu);
uint16_t get_zeropage_y_addr(CPU_6502* cpu);
uint16_t get_indirect_x_addr(CPU_6502* cpu);
uint16_t get_indirect_y_addr(CPU_6502* cpu, uint8_t* extra_cycle);
void lda_flags_do(CPU_6502* cpu);
void lda_immediate(CPU_6502* cpu);
void lda_absolute(CPU_6502* cpu);
void lda_absolute_x(CPU_6502* cpu);
void lda_absolute_y(CPU_6502* cpu);
void lda_zeropage(CPU_6502* cpu);
void lda_zeropage_x(CPU_6502* cpu);
void lda_indirect_x(CPU_6502* cpu);
void lda_indirect_y(CPU_6502* cpu);
void ldx_flags_do(CPU_6502* cpu);
void ldx_immediate(CPU_6502* cpu);
void ldx_absolute(CPU_6502* cpu);
void ldx_absolute_y(CPU_6502* cpu);
void ldx_zeropage(CPU_6502* cpu);
void ldx_zeropage_y(CPU_6502* cpu);
void ldy_flags_do(CPU_6502* cpu);
void ldy_immediate(CPU_6502* cpu);
void ldy_absolute(CPU_6502* cpu);
void ldy_absolute_y(CPU_6502* cpu);
void ldy_zeropage(CPU_6502* cpu);
void ldy_zeropage_y(CPU_6502* cpu);
void sta_absolute(CPU_6502* cpu);
void sta_absolute_x(CPU_6502* cpu);
void sta_absolute_y(CPU_6502* cpu);
void sta_zeropage(CPU_6502* cpu);
void sta_zeropage_x(CPU_6502* cpu);
void sta_indirect_x(CPU_6502* cpu);
void sta_indirect_y(CPU_6502* cpu);
void stx_absolute(CPU_6502* cpu);
void stx_zeropage(CPU_6502* cpu);
void stx_zeropage_y(CPU_6502* cpu);
void sty_absolute(CPU_6502* cpu);
void sty_zeropage(CPU_6502* cpu);
void sty_zeropage_x(CPU_6502* cpu);
void tax_flags_do(CPU_6502* cpu);
void tax_implied(CPU_6502* cpu);
void tay_flags_do(CPU_6502* cpu);
void tay_implied(CPU_6502* cpu);
void tsx_flags_do(CPU_6502* cpu);
void tsx_implied(CPU_6502* cpu);
void txa_flags_do(CPU_6502* cpu);
void txa_implied(CPU_6502* cpu);
void txs_implied(CPU_6502* cpu);
void tya_flags_do(CPU_6502* cpu);
void tya_implied(CPU_6502* cpu);
void pha_implied(CPU_6502* cpu);
void php_implied(CPU_6502* cpu);
void pla_flags_do(CPU_6502* cpu);
void pla_implied(CPU_6502* cpu);
void plp_implied(CPU_6502* cpu);
void and_flags_do(CPU_6502* cpu);
void and_immediate(CPU_6502* cpu);
void and_absolute(CPU_6502* cpu);
void and_absolute_x(CPU_6502* cpu);
void and_absolute_y(CPU_6502* cpu);
void and_zeropage(CPU_6502* cpu);
void and_zeropage_x(CPU_6502* cpu);
void and_indirect_x(CPU_6502* cpu);
void and_indirect_y(CPU_6502* cpu);
void bit_flags_do(CPU_6502* cpu, uint8_t m, uint8_t bband);
void bit_absolute(CPU_6502* cpu);
void bit_zeropage(CPU_6502* cpu);
void eor_flags_do(CPU_6502* cpu);
void eor_immediate(CPU_6502* cpu);
void eor_absolute(CPU_6502* cpu);
void eor_absolute_x(CPU_6502* cpu);
void eor_absolute_y(CPU_6502* cpu);
void eor_zeropage(CPU_6502* cpu);
void eor_zeropage_x(CPU_6502* cpu);
void eor_indirect_x(CPU_6502* cpu);
void eor_indirect_y(CPU_6502* cpu);
void ora_flags_do(CPU_6502* cpu);
void ora_immediate(CPU_6502* cpu);
void ora_absolute(CPU_6502* cpu);
void ora_absolute_x(CPU_6502* cpu);
void ora_absolute_y(CPU_6502* cpu);
void ora_zeropage(CPU_6502* cpu);
void ora_zeropage_x(CPU_6502* cpu);
void ora_indirect_x(CPU_6502* cpu);
void ora_indirect_y(CPU_6502* cpu);
void adc_flags_and_store(CPU_6502* cpu, uint8_t operand);
void adc_immediate(CPU_6502* cpu);
void adc_zeropage(CPU_6502* cpu);
void adc_zeropage_x(CPU_6502* cpu);
void adc_absolute(CPU_6502* cpu);
void adc_absolute_x(CPU_6502* cpu);
void adc_absolute_y(CPU_6502* cpu);
void adc_indirect_x(CPU_6502* cpu);
void adc_indirect_y(CPU_6502* cpu);
void cmp_flags_do(CPU_6502* cpu, uint8_t operand);
void cmp_immediate(CPU_6502* cpu);
void cmp_zeropage(CPU_6502* cpu);
void cmp_zeropage_x(CPU_6502* cpu);
void cmp_absolute(CPU_6502* cpu);
void cmp_absolute_x(CPU_6502* cpu);
void cmp_absolute_y(CPU_6502* cpu);
void cmp_indirect_x(CPU_6502* cpu);
void cmp_indirect_y(CPU_6502* cpu);
void cpx_flags_do(CPU_6502* cpu, uint8_t operand);
void cpx_immediate(CPU_6502* cpu);
void cpx_zeropage(CPU_6502* cpu);
void cpx_absolute(CPU_6502* cpu);
void cpy_flags_do(CPU_6502* cpu, uint8_t operand);
void cpy_immediate(CPU_6502* cpu);
void cpy_zeropage(CPU_6502* cpu);
void cpy_absolute(CPU_6502* cpu);
void sbc_flags_and_store(CPU_6502* cpu, uint8_t operand);
void sbc_immediate(CPU_6502* cpu);
void sbc_zeropage(CPU_6502* cpu);
void sbc_zeropage_x(CPU_6502* cpu);
void sbc_absolute(CPU_6502* cpu);
void sbc_absolute_x(CPU_6502* cpu);
void sbc_absolute_y(CPU_6502* cpu);
void sbc_indirect_x(CPU_6502* cpu);
void sbc_indirect_y(CPU_6502* cpu);
void dec_flags_do(CPU_6502* cpu, uint8_t m);
void dec_absolute(CPU_6502* cpu);
void dec_absolute_x(CPU_6502* cpu);
void dec_zeropage(CPU_6502* cpu);
void dec_zeropage_x(CPU_6502* cpu);
void dex_flags_do(CPU_6502* cpu);
void dex_implied(CPU_6502* cpu);
void dey_flags_do(CPU_6502* cpu);
void dey_implied(CPU_6502* cpu);
void inc_flags_do(CPU_6502* cpu, uint8_t m);
void inc_absolute(CPU_6502* cpu);
void inc_absolute_x(CPU_6502* cpu);
void inc_zeropage(CPU_6502* cpu);
void inc_zeropage_x(CPU_6502* cpu);
void inx_flags_do(CPU_6502* cpu);
void inx_implied(CPU_6502* cpu);
void iny_flags_do(CPU_6502* cpu);
void iny_implied(CPU_6502* cpu);
void asl_flags_do(CPU_6502* cpu, uint8_t m);
void asl_accumulator(CPU_6502* cpu);
void asl_absolute(CPU_6502* cpu);
void asl_absolute_x(CPU_6502* cpu);
void asl_zeropage(CPU_6502* cpu);
void asl_zeropage_x(CPU_6502* cpu);
void lsr_flags_do(CPU_6502* cpu, uint8_t m);
void lsr_accumulator(CPU_6502* cpu);
void lsr_absolute(CPU_6502* cpu);
void lsr_absolute_x(CPU_6502* cpu);
void lsr_zeropage(CPU_6502* cpu);
void lsr_zeropage_x(CPU_6502* cpu);
void rol_flags_do(CPU_6502* cpu, uint8_t m);
void rol_accumulator(CPU_6502* cpu);
void rol_absolute(CPU_6502* cpu);
void rol_absolute_x(CPU_6502* cpu);
void rol_zeropage(CPU_6502* cpu);
void rol_zeropage_x(CPU_6502* cpu);
void ror_flags_do(CPU_6502* cpu, uint8_t m);
void ror_accumulator(CPU_6502* cpu);
void ror_absolute(CPU_6502* cpu);
void ror_absolute_x(CPU_6502* cpu);
void ror_zeropage(CPU_6502* cpu);
void ror_zeropage_x(CPU_6502* cpu);
void brk_implied(CPU_6502* cpu);
void jmp_absolute(CPU_6502* cpu);
void jmp_indirect(CPU_6502* cpu);
void jsr_absolute(CPU_6502* cpu);
void rti_implied(CPU_6502* cpu);
void rts_implied(CPU_6502* cpu);
void bcc_implied(CPU_6502* cpu);
void bcs_implied(CPU_6502* cpu);
void beq_implied(CPU_6502* cpu);
void bmi_implied(CPU_6502* cpu);
void bne_implied(CPU_6502* cpu);
void bpl_implied(CPU_6502* cpu);
void bvc_implied(CPU_6502* cpu);
void bvs_implied(CPU_6502* cpu);
void clc_implied(CPU_6502* cpu);
void sec_implied(CPU_6502* cpu);
void cld_implied(CPU_6502* cpu);
void sed_implied(CPU_6502* cpu);
void cli_implied(CPU_6502* cpu);
void sei_implied(CPU_6502* cpu);
void clv_implied(CPU_6502* cpu);
void nop_implied(CPU_6502* cpu);
void cpu_step(CPU_6502* cpu);

#endif
