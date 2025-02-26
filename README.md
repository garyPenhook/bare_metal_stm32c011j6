# Baremetal STM32C011J6M6 Project

A minimalist bare metal implementation for the STM32C011J6M6 microcontroller without using HAL or any other abstraction libraries.

## Features

- Direct register access for all peripherals
- Minimal startup code with vector table
- Simple LED blinking example
- CMake-based build system
- **Memory usage analysis and visualization**

## Project Structure
```
.
├── CMakeLists.txt             # Main build file
├── cmake/                     # CMake configuration
│   ├── arm-none-eabi.cmake    # Toolchain file
│   └── stm32_linker.ld        # Linker script
├── include/                   # Header files
│   ├── main.h                 # Main header
│   └── stm32c011xx.h          # Device header
├── inc/                       # Additional header files
│   ├── stm32_peripherals.h    # Peripheral definitions
│   └── tick.h                 # Timing functions
├── src/                       # Source files
│   ├── main.c                 # Application code
│   ├── startup.c              # Vector table and reset handler
│   └── ...                    # Additional source files
├── tools/                     # Utility scripts
│   └── memory_usage.py        # Memory usage visualization
└── stm32_linker.ld            # Linker script (duplicate)
```

## Building

```bash
mkdir -p build
cd build
cmake ..
make
```

## Memory Usage Analysis

The project includes several ways to analyze memory usage:

1. **Automatic Analysis**: Memory usage is displayed after each build
2. **VS Code Task**: Run the "Analyze Memory Usage" task to see memory statistics
3. **Visual Analysis**: Run the "Visual Memory Analysis" task to see a graphical representation

### Memory Analysis Output Example

```
=======================================================
Memory Usage for stm32_project.elf
=======================================================
Flash: 2468 / 32768 bytes (7.53%)
RAM:   108 / 6144 bytes (1.76%)
=======================================================
```

## Flashing

Using ST-Link:
```bash
st-flash write build/stm32_project.bin 0x08000000
```

Using OpenOCD:
```bash
openocd -f interface/stlink.cfg -f target/stm32c0x.cfg -c 'program build/stm32_project.elf verify reset exit'
```

## Debugging

1. Start OpenOCD:
```bash
openocd -f interface/stlink.cfg -f target/stm32c0x.cfg
```

2. Connect with GDB:
```bash
arm-none-eabi-gdb build/stm32_project.elf
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) break main
(gdb) continue
```

## Hardware Connections

For this example, connect:
- An LED with a suitable resistor to pin PA13
- ST-Link programmer to the appropriate pins (SWDIO, SWCLK, GND, VDD)