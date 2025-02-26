#!/usr/bin/env python3
"""
Memory Usage Visualizer for STM32 Projects
This script analyzes the memory usage of an STM32 ELF file and
creates a visual representation of Flash and RAM usage.
"""

import sys
import subprocess
import os
import re
from collections import defaultdict

# ANSI colors for terminal output
class Colors:
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'

def run_size_command(elf_file):
    """Run arm-none-eabi-size and parse the output"""
    try:
        output = subprocess.check_output(['arm-none-eabi-size', '-A', '-d', elf_file]).decode('utf-8')
        return output
    except subprocess.CalledProcessError:
        print(f"{Colors.RED}Error: Failed to run arm-none-eabi-size{Colors.ENDC}")
        sys.exit(1)
    except FileNotFoundError:
        print(f"{Colors.RED}Error: arm-none-eabi-size not found. Make sure ARM toolchain is installed.{Colors.ENDC}")
        sys.exit(1)

def parse_memory_info(size_output):
    """Parse the size command output and organize by memory region"""
    memory_info = defaultdict(int)

    # Regular expression to match section lines
    section_regex = re.compile(r'^\s*\.(\w+)\s+(\d+)')

    for line in size_output.split('\n'):
        match = section_regex.match(line)
        if match:
            section_name = match.group(1)
            section_size = int(match.group(2))

            # Categorize sections by memory region (Flash or RAM)
            if section_name in ['text', 'rodata', 'isr_vector', 'vector_table']:
                memory_info['Flash'] += section_size
            elif section_name in ['data']:
                # Data goes both in Flash (for init values) and RAM (for runtime)
                memory_info['Flash'] += section_size
                memory_info['RAM'] += section_size
            elif section_name in ['bss']:
                memory_info['RAM'] += section_size

    return memory_info

def draw_progress_bar(used, total, label, color):
    """Draw a colored progress bar"""
    width = 50  # Width of the progress bar in characters
    percent = used * 100.0 / total
    filled_length = int(width * used / total)

    bar = '█' * filled_length + '░' * (width - filled_length)

    # Choose color based on usage percentage
    if percent < 60:
        bar_color = Colors.GREEN
    elif percent < 80:
        bar_color = Colors.YELLOW
    else:
        bar_color = Colors.RED

    print(f"{color}{label}:{Colors.ENDC} [{bar_color}{bar}{Colors.ENDC}] {used}/{total} bytes ({percent:.2f}%)")

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <elf-file>")
        sys.exit(1)

    elf_file = sys.argv[1]
    if not os.path.exists(elf_file):
        print(f"{Colors.RED}Error: File {elf_file} not found{Colors.ENDC}")
        sys.exit(1)

    # STM32C011J6M6 memory sizes
    flash_size = 32 * 1024  # 32 KB
    ram_size = 6 * 1024     # 6 KB

    # Run size command and parse output
    size_output = run_size_command(elf_file)
    memory_info = parse_memory_info(size_output)

    print(f"\n{Colors.BOLD}Memory Usage Analysis for {elf_file}{Colors.ENDC}")
    print(f"{Colors.BOLD}{'=' * 60}{Colors.ENDC}")

    # Draw progress bars
    draw_progress_bar(memory_info['Flash'], flash_size, "Flash", Colors.BLUE)
    draw_progress_bar(memory_info['RAM'], ram_size, "RAM  ", Colors.GREEN)

    print(f"{Colors.BOLD}{'=' * 60}{Colors.ENDC}")
    print(f"STM32C011J6M6: 32KB Flash, 6KB RAM\n")

if __name__ == "__main__":
    main()
