
CC=gcc

deps:


	sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev    grub-common xorriso  nasm  grub-pc-bin libfontconfig1 mesa-common-dev  libglu1-mesa-dev -y

mkiso:


	as --32 boot.s -o boot.o

	${CC} -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
	
	${CC} -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
	
	${CC} -m32 -c char.c -o char.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
	
	${CC}  -m32 -c logo.c -o logo.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra
	
	ld -m elf_i386 -T linker.ld kernel.o utils.o char.o logo.o boot.o -o charleX.bin -nostdlib
	
	grub-file --is-x86-multiboot charleX.bin
	
	mkdir -p iso/boot/grub
	
	
	cp charleX.bin iso/boot/charleX.bin
	
	cp grub.cfg iso/boot/grub/grub.cfg
	
	
	grub-mkrescue -o charleX.iso iso
	
	rm -rf *.bin *.o
clean:
	rm -rf *.o *.bin
