#!/bin/bash

# Parse ELF file size information
SIZE_OUTPUT=$(arm-none-eabi-size -A -d $1)

# Extract text (Flash) and data+bss (RAM) sections
TEXT_SIZE=$(echo "$SIZE_OUTPUT" | grep '\.text' | awk '{print $2}')
RODATA_SIZE=$(echo "$SIZE_OUTPUT" | grep '\.rodata' | awk '{print $2}')
# Some firmware may have an isr_vector section separate from .text
VECTOR_SIZE=$(echo "$SIZE_OUTPUT" | grep -E '\.isr_vector|\.vector_table' | awk '{print $2}')

# Data and BSS sections (RAM)
DATA_SIZE=$(echo "$SIZE_OUTPUT" | grep '\.data' | awk '{print $2}')
BSS_SIZE=$(echo "$SIZE_OUTPUT" | grep '\.bss' | awk '{print $2}')

# Handle case where some sections might not exist
TEXT_SIZE=${TEXT_SIZE:-0}
RODATA_SIZE=${RODATA_SIZE:-0}
VECTOR_SIZE=${VECTOR_SIZE:-0}
DATA_SIZE=${DATA_SIZE:-0}
BSS_SIZE=${BSS_SIZE:-0}

# Total Flash and RAM usage
FLASH_USED=$((TEXT_SIZE + RODATA_SIZE + VECTOR_SIZE + DATA_SIZE))
RAM_USED=$((DATA_SIZE + BSS_SIZE))

# Memory sizes from linker script (in bytes)
FLASH_SIZE=32768  # 32KB
RAM_SIZE=6144     # 6KB

# Calculate percentages
FLASH_PERCENT=$(echo "scale=2; $FLASH_USED * 100 / $FLASH_SIZE" | bc)
RAM_PERCENT=$(echo "scale=2; $RAM_USED * 100 / $RAM_SIZE" | bc)

echo "======================================================="
echo "Memory Usage for $1"
echo "======================================================="
echo "Flash: $FLASH_USED / $FLASH_SIZE bytes ($FLASH_PERCENT%)"
echo "RAM:   $RAM_USED / $RAM_SIZE bytes ($RAM_PERCENT%)"
echo "======================================================="
