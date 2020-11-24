#!/bin/bash

#assemble boot.s file
as --32 boot.s -o boot.o

#compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#compile about.c file
gcc -m32 -c about.c -o about.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#compile virtual.c file
gcc -m32 -c virtual.c -o virtual.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
#compile virtual.c file
gcc -m32 -c qemuTestVM.c -o qemuTestVM.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#comiple other (utils.c, char.c,...)

gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
gcc -m32 -c char.c -o char.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
gcc -m32 -c logo.c -o logo.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#linking the kernel with kernel.o and boot.o files and other file ( virtual and about and qemu)

ld -m elf_i386 -T linker.ld kernel.o utils.o char.o logo.o boot.o -o charleX.bin -nostdlib
ld -m elf_i386 -T linker.ld about.o boot.o logo.o  -o   about.bin -nostdlib
ld -m elf_i386 -T linker.ld virtual.o utils.o char.o boot.o logo.o  -o virtual.bin -nostdlib
ld -m elf_i386 -T linker.ld qemuTestVM.o utils.o char.o boot.o logo.o  -o qemuTestVM.bin -nostdlib


#check charleX.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot charleX.bin

#building the iso file
mkdir -p iso/boot/grub
mkdir -p iso/about/
mkdir -p iso/virtual/
mkdir -p iso/qemu/
cp charleX.bin iso/boot/charleX.bin
cp about.bin iso/about/about.bin
cp virtual.bin iso/virtual/virtual.bin
cp qemuTestVM.bin iso/qemu/qemuTestVM.bin
cp grub.cfg iso/boot/grub/grub.cfg
grub-mkrescue -o charleX.iso iso
rm *.o *.bin
#run it in qemu
qemu-system-i386 charleX.iso
