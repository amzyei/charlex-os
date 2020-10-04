
# charleX OS logo

<pre>
                                                          ,/{}
                                                        ,/  {|
                                                    ,,,/    {|,
                                              __--~~        {| ~-,
                                        __--~~              {     `\\
                                                                ,__ \\
          888                       888           Y88b    /   `,\\{),\\,
   e88~~\\ 888-~88e   /~~~8e  888-~\\ 888  e88~~8e   Y88b  /   __-~  `_ ~-_
  d888    888  888       88b 888    888 d888  88b   Y88b/  _-~        ~~-_`~-_
  8888    888  888  e88~-888 888    888 8888__888   /Y88b '             `~-_`~-__ 
  Y888    888  888 C888  888 888    888 Y888    ,  /  Y88b`,                `~-\\_|
   \"88__/ 888  888  \"88_-888 888    888  \"88___/  /    Y88b`,     _-----___    _,'
                                                           / /--__  ~~--__  `~,~
                                                            /     ~~--__  ~-',
  ------------------------------------------------------' 
</pre>

# charleX OS
charleX is a simple OS ... kernel written with c programming language and reference of this code is 'codeproject.com'. 
I still working to make a better kernel than this one, and many features will be added to this project. 
I wanna create the free and open-source OS like gnu/Linux, *BSD,... and you can support us. if you like this project, give a star to the project and send better code to us.
# compile and run
$as --32 boot.s -o boot.o

$gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$gcc -m32 -c char.c -o char.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$ld -m elf_i386 -T linker.ld kernel.o utils.o char.o boot.o -o charleX.bin -nostdlib

$grub-file --is-x86-multiboot charleX.bin

$mkdir -p iso/boot/grub

$cp charleX.bin iso/boot/charleX.bin

$cp grub.cfg iso/boot/grub/grub.cfg

$grub-mkrescue -o charleX.iso iso

$qemu-system-i386 charleX.iso

# Automatic compile and run

$chmod +x makefile

$./makefile


# GCC Cross Compiler 

$chmod +x ./gccCrossCompiler

$./gccCrossComplier (apt,yum,dnf,...)

# report bug or problems

send bug or anythings : amzy0zone@gmail.com

thanks for reading this ! :)
