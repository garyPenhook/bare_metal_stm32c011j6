# Create a README.md file
cat > README.md << 'EOF'
# Baremetal STM32C011J6M6 Project

A minimalist bare metal implementation for the STM32C011J6M6 microcontroller without using HAL or any other abstraction libraries.

## Features

- Direct register access for all peripherals
- Minimal startup code with vector table
- Simple LED blinking example
- CMake-based build system

## Building

```bash
mkdir -p build
cd build
cmake ..
make