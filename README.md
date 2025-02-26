# Baremetal STM32C011J6M6 Project

A minimalist bare metal implementation for the STM32C011J6M6 microcontroller without using HAL or any other abstraction libraries.

## Features

- Direct register access for all peripherals
- Minimal startup code with vector table
- Simple LED blinking example
- CMake-based build system

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
├── src/                       # Source files
│   ├── main.c                 # Application code
│   └── startup.c              # Vector table and reset handler
└── stm32_linker.ld            # Linker script (duplicate)
```

## Building

```bash
mkdir -p build
cd build
cmake ..
make
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
- An LED with a suitable resistor to pin PA5
- ST-Link programmer to the appropriate pins (SWDIO, SWCLK, GND, VDD)
## Updated on Wed 26 Feb 2025 12:33:55 AM EST
