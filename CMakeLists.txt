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

# Generate hex and bin files
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf
)