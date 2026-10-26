CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Isource/CPU_6502

SOURCES = $(wildcard source/*.c) $(wildcard source/CPU_6502/*.c)
OBJECTS = $(patsubst source/%.c,build/%.o,$(SOURCES))
TARGET = build/nes-emulator

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

build/%.o: source/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build