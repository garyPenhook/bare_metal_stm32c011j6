
cmake_minimum_required(VERSION 3.20)

# Include custom toolchain file first, before project()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi.cmake CACHE FILEPATH "ARM Toolchain file")

# Project name
project(stm32_project C ASM)

# Set the microcontroller details for STM32C011J6M6
set(MCU_FAMILY STM32C0xx)
set(MCU_MODEL STM32C011xx)
set(CPU_PARAMETERS "-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft")

# Set compiler flags - added -Os for size optimization
set(CMAKE_C_FLAGS "${CPU_PARAMETERS} -Wall -Wextra -Werror=implicit-function-declaration -Wno-unused-parameter -Os")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
# Explicitly specify NOT to use nano.specs
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nosys.specs")

# Fix linker flags for baremetal - avoid standard startup files but keep libraries
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -nostartfiles")

# Set build type to Debug by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories - ensure all paths are available
include_directories(
    include
    inc
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/inc
)

# Gather source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Set linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/cmake/stm32_linker.ld)
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-T${LINKER_SCRIPT}")

# Link with standard libraries (not nano versions)
target_link_libraries(${PROJECT_NAME}.elf gcc c nosys m)

# Create memory usage script
file(WRITE ${CMAKE_BINARY_DIR}/memory_usage.sh "#!/bin/bash
# Parse ELF file size information
SIZE_OUTPUT=\$(arm-none-eabi-size -A -d \$1)

# Extract text (Flash) and data+bss (RAM) sections
TEXT_SIZE=\$(echo \"\$SIZE_OUTPUT\" | grep '\\.text' | awk '{print \$2}')
RODATA_SIZE=\$(echo \"\$SIZE_OUTPUT\" | grep '\\.rodata' | awk '{print \$2}')
# Some firmware may have an isr_vector section separate from .text
VECTOR_SIZE=\$(echo \"\$SIZE_OUTPUT\" | grep -E '\\.isr_vector|\\.vector_table' | awk '{print \$2}')

# Data and BSS sections (RAM)
DATA_SIZE=\$(echo \"\$SIZE_OUTPUT\" | grep '\\.data' | awk '{print \$2}')
BSS_SIZE=\$(echo \"\$SIZE_OUTPUT\" | grep '\\.bss' | awk '{print \$2}')

# Handle case where some sections might not exist
TEXT_SIZE=\${TEXT_SIZE:-0}
RODATA_SIZE=\${RODATA_SIZE:-0}
VECTOR_SIZE=\${VECTOR_SIZE:-0}
DATA_SIZE=\${DATA_SIZE:-0}
BSS_SIZE=\${BSS_SIZE:-0}

# Total Flash and RAM usage
FLASH_USED=\$((TEXT_SIZE + RODATA_SIZE + VECTOR_SIZE + DATA_SIZE))
RAM_USED=\$((DATA_SIZE + BSS_SIZE))

# Memory sizes from linker script (in bytes)
FLASH_SIZE=32768  # 32KB
RAM_SIZE=6144     # 6KB

# Calculate percentages
FLASH_PERCENT=\$(echo \"scale=2; \$FLASH_USED * 100 / \$FLASH_SIZE\" | bc)
RAM_PERCENT=\$(echo \"scale=2; \$RAM_USED * 100 / \$RAM_SIZE\" | bc)

echo \"=======================================================\"
echo \"Memory Usage for \$1\"
echo \"=======================================================\"
echo \"Flash: \$FLASH_USED / \$FLASH_SIZE bytes (\$FLASH_PERCENT%)\"
echo \"RAM:   \$RAM_USED / \$RAM_SIZE bytes (\$RAM_PERCENT%)\"
echo \"=======================================================\"
")

# Make script executable
execute_process(
    COMMAND chmod +x ${CMAKE_BINARY_DIR}/memory_usage.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Generate hex and bin files, and execute memory usage script
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND bash ${CMAKE_BINARY_DIR}/memory_usage.sh ${PROJECT_NAME}.elf
)

# Add dedicated memory analysis target
add_custom_target(memory_analysis
    COMMAND ${CMAKE_BINARY_DIR}/memory_usage.sh ${PROJECT_NAME}.elf
    DEPENDS ${PROJECT_NAME}.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Analyzing memory usage..."
)

# Add a custom command to run after build
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND echo "======= Memory Usage Analysis ======="
    COMMAND ${CMAKE_BINARY_DIR}/memory_usage.sh ${PROJECT_NAME}.elf
    COMMAND echo "====================================="
)