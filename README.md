# NES Emulator

<p align="center">
    <img src="docs/images/nes.jpg" alt="NES Emulator" width="840">
</p>

An NES emulator written in C. The project is currently focused on implementing the MOS 6502 CPU and the NES memory system.

## Current status

The emulator is under development.

Currently, the project contains:
- An implementation of the documented 6502 instructions used by the test ROM.
- Major stable unofficial 6502 instruction families.
- CPU instruction and addressing-mode code.
- CPU status flags, stack operations and cycle counting.
- A test harness using a NES test ROM.
- A browser-based CPU debugger for visualizing execution traces.
- A basic NES memory bus.
- 2 KB internal RAM with address mirroring.
- PRG ROM mapping for 16 KB and 32 KB cartridges.
- Basic iNES ROM loading with NROM (Mapper 0) support.

The CPU implementation successfully passes all 8991 instruction checks in `nestest`.

The following components are not implemented yet:
- PPU (graphics processing).
- APU (audio processing).
- Controllers.
- Additional cartridge mappers.
- Complete NES memory and hardware integration.

The emulator is not yet intended to run arbitrary NES games.

## Requirements

- GCC or another compatible C compiler.
- GNU Make.
- Python 3 (for the browser-based debugger).
- A compatible `nestest.nes` test ROM.
- The corresponding `nestest.log` reference file.

The test ROM and reference log are not included in this repository. Obtain them from sources you are legally permitted to use.

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

Place `nestest.nes` and `nestest.log` in the repository root, then run:

```sh
./build/nes-emulator
```

The program compares CPU execution against the reference log and generates `trace.json` for the debugger.

The current program expects the test files at the paths specified in the source code. Passing filenames through command-line arguments is not implemented yet.

## CPU Debugger

The project includes a browser-based debugger for visualizing the recorded 6502 CPU execution trace.

The debugger provides:
- Step-by-step instruction playback.
- CPU register and status flag inspection.
- Register change visualization.
- Instruction opcode and mnemonic display.
- Execution history.
- Playback speed control and timeline navigation.

### Launching the debugger

First, build and run the emulator to generate `trace.json`.

Then start a local HTTP server from the repository root:

```sh
python3 -m http.server 8000
```

Open the following address in your browser:

```text
http://localhost:8000/debugger.html
```

The debugger automatically loads `trace.json`.

The trace file is generated locally and should not be committed to the repository.

## Project structure

```text
NES-Emulator/
├── docs/
│   └── images/
│       └── nes.jpg
├── source/
│   ├── main.c
│   ├── CPU_6502/
│   │   ├── 6502.c
│   │   └── 6502.h
│   └── bus/
│       ├── bus.c
│       └── bus.h
├── tests/
│   └── test_bus.c
├── debugger.html
├── .gitignore
├── Makefile
└── README.md
```

Build output is stored in `build/` and should not be committed.

## Development

Keep source code and build configuration in Git.

Do not commit compiled executables, object files, generated trace files, or ROM files.

Build and test changes locally before committing them.