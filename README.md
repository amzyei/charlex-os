# charleX OS

```
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
```

## About charleX OS

charleX OS is a simple and free operating system. The main part, called the kernel, is written in the C language. The project is still being worked on. The goal is to make an easy-to-use and fast operating system. It wants to be like Windows but also fast like Linux or FreeBSD.

## Features

- Simple kernel written in C
- Free and open source
- Easy to use and fast
- Designed to add new features easily

## What you need

To build charleX OS, you need to have these tools:

- GNU Make
- GCC (C Compiler)
- NASM (Assembler)
- GRUB bootloader tools

## How to build

Run these commands in the main project folder:

```bash
make deps    # Install needed tools
make mkiso   # Build the ISO file
make clean   # Remove build files
```

## How to run

After building, you can run the ISO file in a virtual machine like VirtualBox or QEMU.

## Screenshots

You can see pictures of charleX OS in the `screenshots/` folder. They show the boot screen, loading screen, and command prompt.

## How to help

You can help by sending your changes. Fork the project, make your changes, and send a pull request. Please follow the current style and add comments.

## License

charleX OS uses the license in the [LICENSE](./LICENSE) file.
