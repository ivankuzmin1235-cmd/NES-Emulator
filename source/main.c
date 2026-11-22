#include "6502.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE "nestest.log"

typedef struct {
    unsigned int pc;
    unsigned int a;
    unsigned int x;
    unsigned int y;
    unsigned int p;
    unsigned int sp;
    unsigned long long cycles;
} ExpectedState;

/*
 * Читаем одну строку nestest.log
 * и извлекаем из неё ожидаемое состояние CPU.
 */
static int parse_log_line(const char* line, ExpectedState* expected) {
    const char* state = strstr(line, "A:");

    if (state == NULL) {
        return 0;
    }

    if (sscanf(line, "%x", &expected->pc) != 1) {
        return 0;
    }

    if (sscanf(
        state,
        "A:%x X:%x Y:%x P:%x SP:%x PPU: %*u, %*u CYC:%llu",
        &expected->a,
        &expected->x,
        &expected->y,
        &expected->p,
        &expected->sp,
        &expected->cycles
    ) != 6) {
        return 0;
    }

    return 1;
}

/*
 * Сравниваем текущее состояние CPU
 * с эталонным состоянием из nestest.log.
 */
static int compare_cpu_state(
    const CPU_6502* cpu,
    const ExpectedState* expected,
    unsigned long step
) {
    if (cpu->pc != expected->pc ||
        cpu->a != expected->a ||
        cpu->x != expected->x ||
        cpu->y != expected->y ||
        cpu->p != expected->p ||
        cpu->sp != expected->sp ||
        cpu->cycles != expected->cycles) {

        printf("\nTEST FAILED at step %lu!\n\n", step);

        printf("Expected:\n");
        printf("PC=$%04X A=$%02X X=$%02X Y=$%02X "
               "P=$%02X SP=$%02X CYC=%llu\n",
               expected->pc,
               expected->a,
               expected->x,
               expected->y,
               expected->p,
               expected->sp,
               expected->cycles);

        printf("\nActual:\n");
        printf("PC=$%04X A=$%02X X=$%02X Y=$%02X "
               "P=$%02X SP=$%02X CYC=%llu\n",
               cpu->pc,
               cpu->a,
               cpu->x,
               cpu->y,
               cpu->p,
               cpu->sp,
               (unsigned long long)cpu->cycles);

        return 0;
    }

    return 1;
}

/*
 * Последовательно исполняем инструкции,
 * сверяя каждое состояние с эталонным логом.
 */
static int run_cpu_test(CPU_6502* cpu) {
    FILE* log = fopen(LOG_FILE, "r");

    if (log == NULL) {
        printf("ERROR: Cannot open %s\n", LOG_FILE);
        return 0;
    }

    char line[256];
    ExpectedState expected;
    unsigned long step = 0;

    printf("Starting nestest comparison...\n\n");

    while (fgets(line, sizeof(line), log) != NULL) {

        if (!parse_log_line(line, &expected)) {
            printf("ERROR: Cannot parse log at step %lu\n", step);
            fclose(log);
            return 0;
        }

        if (!compare_cpu_state(cpu, &expected, step)) {
            fclose(log);
            return 0;
        }

        cpu_step(cpu);
        step++;
    }

    if (ferror(log)) {
        printf("ERROR: Failed to read %s\n", LOG_FILE);
        fclose(log);
        return 0;
    }

    fclose(log);

    printf("ALL TESTS PASSED!\n");
    printf("Successfully compared %lu instructions.\n", step);

    return 1;
}

int main(void) {
    CPU_6502 cpu;

    cpu_init(&cpu, "nestest.nes");

    /*
     * Временно пропускаем начальную процедуру загрузки NES.
     * Начинаем исполнение непосредственно с тестовой программы.
     */
    cpu.pc = 0xC000;

    int test_passed = run_cpu_test(&cpu);

    return test_passed ? 0 : 1;
}
