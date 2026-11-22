
#include "6502.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define LOG_FILE "nestest.log"
#define TRACE_FILE "trace.json"

typedef struct {
    unsigned int pc;
    unsigned int a;
    unsigned int x;
    unsigned int y;
    unsigned int p;
    unsigned int sp;
    unsigned long long cycles;
} ExpectedState;

typedef struct {
    uint16_t pc;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t p;
    uint8_t sp;
    uint64_t cycles;
} CPUState;

static CPUState get_cpu_state(const CPU_6502* cpu) {
    CPUState state = {
        .pc = cpu->pc,
        .a = cpu->a,
        .x = cpu->x,
        .y = cpu->y,
        .p = cpu->p,
        .sp = cpu->sp,
        .cycles = cpu->cycles
    };

    return state;
}

static int parse_log_line(
    const char* line,
    ExpectedState* expected
) {
    const char* state = strstr(line, "A:");

    if (state == NULL) {
        return 0;
    }

    if (sscanf(line, "%x", &expected->pc) != 1) {
        return 0;
    }

    return sscanf(
        state,
        "A:%x X:%x Y:%x P:%x SP:%x PPU: %*u, %*u CYC:%llu",
        &expected->a,
        &expected->x,
        &expected->y,
        &expected->p,
        &expected->sp,
        &expected->cycles
    ) == 6;
}

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
        printf(
            "PC=$%04X A=$%02X X=$%02X Y=$%02X "
            "P=$%02X SP=$%02X CYC=%llu\n",
            expected->pc,
            expected->a,
            expected->x,
            expected->y,
            expected->p,
            expected->sp,
            expected->cycles
        );

        printf("\nActual:\n");
        printf(
            "PC=$%04X A=$%02X X=$%02X Y=$%02X "
            "P=$%02X SP=$%02X CYC=%" PRIu64 "\n",
            (unsigned int)cpu->pc,
            (unsigned int)cpu->a,
            (unsigned int)cpu->x,
            (unsigned int)cpu->y,
            (unsigned int)cpu->p,
            (unsigned int)cpu->sp,
            (uint64_t)cpu->cycles
        );

        return 0;
    }

    return 1;
}

static int write_trace_entry(
    FILE* trace,
    unsigned long step,
    uint16_t pc,
    uint8_t opcode,
    uint8_t byte1,
    uint8_t byte2,
    const CPUState* before,
    const CPUState* after
) {
    int result = fprintf(
        trace,

        "%s"
        "{\"step\":%lu,"
        "\"pc\":%u,"
        "\"bytes\":[%u,%u,%u],"

        "\"before\":{"
            "\"a\":%u,\"x\":%u,\"y\":%u,"
            "\"p\":%u,\"sp\":%u,\"pc\":%u,"
            "\"cycles\":%" PRIu64
        "},"

        "\"after\":{"
            "\"a\":%u,\"x\":%u,\"y\":%u,"
            "\"p\":%u,\"sp\":%u,\"pc\":%u,"
            "\"cycles\":%" PRIu64
        "}}",

        step == 0 ? "" : ",\n",

        step,
        (unsigned int)pc,

        (unsigned int)opcode,
        (unsigned int)byte1,
        (unsigned int)byte2,

        (unsigned int)before->a,
        (unsigned int)before->x,
        (unsigned int)before->y,
        (unsigned int)before->p,
        (unsigned int)before->sp,
        (unsigned int)before->pc,
        before->cycles,

        (unsigned int)after->a,
        (unsigned int)after->x,
        (unsigned int)after->y,
        (unsigned int)after->p,
        (unsigned int)after->sp,
        (unsigned int)after->pc,
        after->cycles
    );

    return result >= 0;
}

static int run_cpu_test(CPU_6502* cpu) {
    FILE* log = fopen(LOG_FILE, "r");

    if (log == NULL) {
        printf("ERROR: Cannot open %s\n", LOG_FILE);
        return 0;
    }

    FILE* trace = fopen(TRACE_FILE, "w");

    if (trace == NULL) {
        printf("ERROR: Cannot create %s\n", TRACE_FILE);
        fclose(log);
        return 0;
    }

    int test_passed = 1;

    if (fprintf(trace, "[\n") < 0) {
        printf("ERROR: Cannot write to %s\n", TRACE_FILE);
        fclose(log);
        fclose(trace);
        return 0;
    }

    char line[256];

    ExpectedState expected;
    unsigned long step = 0;

    printf("Starting nestest comparison...\n\n");

    while (fgets(line, sizeof(line), log) != NULL) {

        if (!parse_log_line(line, &expected)) {
            printf("ERROR: Cannot parse log at step %lu\n", step);
            test_passed = 0;
            break;
        }

        if (!compare_cpu_state(cpu, &expected, step)) {
            test_passed = 0;
            break;
        }

        uint16_t pc = cpu->pc;

        uint8_t opcode = cpu_read(cpu, pc);
        uint8_t byte1 = cpu_read(cpu, (uint16_t)(pc + 1));
        uint8_t byte2 = cpu_read(cpu, (uint16_t)(pc + 2));

        CPUState before = get_cpu_state(cpu);

        cpu_step(cpu);

        CPUState after = get_cpu_state(cpu);

        if (!write_trace_entry(
            trace,
            step,
            pc,
            opcode,
            byte1,
            byte2,
            &before,
            &after
        )) {
            printf("ERROR: Failed to write trace at step %lu\n", step);
            test_passed = 0;
            break;
        }

        step++;
    }

    if (ferror(log)) {
        printf("ERROR: Failed to read %s\n", LOG_FILE);
        test_passed = 0;
    }

    if (step == 0 && test_passed) {
        printf("ERROR: No instructions were tested.\n");
        test_passed = 0;
    }

    if (fprintf(trace, "\n]\n") < 0) {
        printf("ERROR: Failed to finish JSON trace.\n");
        test_passed = 0;
    }

    if (fclose(trace) != 0) {
        printf("ERROR: Failed to close %s\n", TRACE_FILE);
        test_passed = 0;
    }

    fclose(log);

    if (test_passed) {
        printf("\nALL TESTS PASSED!\n");
        printf("Successfully compared %lu instructions.\n", step);
    }

    printf("Trace saved to %s (%lu instructions).\n", TRACE_FILE, step);

    return test_passed;
}


int main(void) {
    CPU_6502 cpu;

    cpu_init(&cpu, "nestest.nes");

    cpu.pc = 0xC000;

    int test_passed = run_cpu_test(&cpu);

    return test_passed ? 0 : 1;
}
