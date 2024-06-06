

# charleX OS logo :


                  %%  %%                      %%  %%      
                 %%      %%                  %%      %%   
                 %%        %%              %%        %%   
                 %%          %%%%%%%%%%%%%%          %%   
                 %%                                  %%   
                 %%                                  %%   
               %%        %%%%              %%%%        %% 
             %%          %%%%%%          %%%%%%          %%
             %%                                          %%
             %%                  %%%%%%                  %%
             %%            %%      %%      %%            %%
               %%            %%%%%%%%%%%%%%            %%  
                %%                                    %%  
                   %%                              %%      
                     %%%%%%%%              %%%%%%%%     
                             %%%%%%%%%%%%%%               



# charleX OS

CharleX is a simple operating system. The kernel is written in C. I am still working on the source code to improve the kernel with many new features. My goal is to create a free, open-source operating system similar to Linux and FreeBSD. You can support us by contributing and improving the code for this project or by giving it a star.


![screenshots](screenshots/charleXGrub.png)

![screenshots](screenshots/charleXLoadingAndLogo.png)

![screenshots](screenshots/charleXPrompt.png)


# compile and run :

$ as --32 boot.s -o boot.o

$ gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$ gcc -m32 -c about.c -o about.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$ gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$ gcc -m32 -c char.c -o char.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$ gcc -m32 -c logo.c -o logo.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

$ ld -m elf_i386 -T linker.ld kernel.o utils.o char.o logo.o boot.o -o charleX.bin -nostdlib

$ ld -m elf_i386 -T linker.ld about.o  utils.o char.o logo.o boot.o -o about.bin -nostdlib

$ grub-file --is-x86-multiboot charleX.bin

$ mkdir -p iso/boot/grub

$ mkdir -p iso/about/

$ cp charleX.bin iso/boot/charleX.bin

$ cp about.bin iso/about/about.bin

$ cp grub.cfg iso/boot/grub/grub.cfg

$ grub-mkrescue -o charleX.iso iso

$ rm *.bin *.o 






# Automatic compile and run

$ chmod +x run.sh

$ ./run.sh


# OS and GCC dependencies(Debian/Ubuntu):

$ chmod +x ./Depends.deb.sh

$./Depends.deb.sh

