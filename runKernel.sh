#!/bin/bash

# Add to PATH
export PATH="$HOME/opt/i686-elf/bin:$PATH"

# Compile 
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
# Link
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o terminal.o -lgcc

# Run the OS image QEMU
qemu-system-i386 -kernel myos.bin
