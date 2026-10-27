# NES Emulator

<p aling ="center">
    <img src="docs/images/nes.jpg" alt="NES Emulator" width="830">
</p>

An NES emulator written in C. The project is currently focused on
implementing the MOS 6502 CPU and testing its instructions.

## Current status

The emulator is under development.

Currently, the project contains:
- A partial implementation of the 6502 CPU.
- CPU instruction and addressing-mode code.
- An initial test harness using a NES test ROM.

The following components are not implemented yet:
- PPU (graphics processing).
- APU (audio processing).
- Controllers.
- Cartridge mappers.
- A complete NES memory bus.

The emulator is not yet intended to run arbitrary NES games.

## Requirements

- GCC or another compatible C compiler.
- GNU Make.
- A compatible `nestest.nes` test ROM.

The ROM is not included in this repository. Obtain it from a source
you are legally permitted to use.

## Build

Run these commands from the repository root:

```sh
make
```

The executable will be created at:

```text
build/nes-emulator
```

To remove generated build files:

```sh
make clean
```

## Run

Place `nestest.nes` in the repository root, then run:

```sh
./build/nes-emulator
```

The current program expects the ROM at the path specified in
`source/main.c`. Passing a ROM filename through command-line
arguments is not implemented yet.

## Project structure

```text
NES-Emulator/
├── .gitignore
├── Makefile
├── README.md
└── source/
    ├── main.c
    └── CPU_6502/
        ├── 6502.c
        └── 6502.h
```

Build output is stored in `build/` and should not be committed.

## Development

Keep source code and build configuration in Git.
Do not commit compiled executables, object files, or ROM files.

Build and test changes locally before committing them.