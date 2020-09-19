#include "includes/kernel.h"
#include "includes/utils.h"
#include "includes/char.h"
uint32 vga_index;
static uint32 next_line_index = 1;
uint8 g_fore_color = WHITE, g_back_color = BLUE;
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/*
this is same as we did in our assembly code for vga_print_char

vga_print_char:
  mov di, word[VGA_INDEX]
  mov al, byte[VGA_CHAR]

  mov ah, byte[VGA_BACK_COLOR]
  sal ah, 4
  or ah, byte[VGA_FORE_COLOR]

  mov [es:di], ax

  ret

*/
uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color)
{
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}


// set terminal color

void init_vga(uint8 fore_color, uint8 back_color)
{
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}


//this function print a new line on your termninal like '\n'

void newline_on_terminal()
{
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}

void print_char(char ch)
{
  vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index++;
}

void print_on_terminal(char *str)
{
  uint32 index =0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

void print_int(int num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_on_terminal(str_num);
}

uint8 inb(uint16 port)
{
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}

void outb(uint16 port, uint8 data)
{
  asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}
// get keys from your keyboard
//and read
char get_input_prompt()
{
  char ch = 0;
  while((ch = inb(KEYBOARD_PORT)) != 0){
    if(ch > 0)
      return ch;
  }
  return ch;
}

/*
keep the cpu busy for doing nothing(nop)
so that io port will not be processed by cpu
here timer can also be used, but lets do this in looping counter
*/
void wait_for_io(uint32 timer_count)
{
  while(1){
    asm volatile("nop" /*do noting*/  );
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}
//sleep function
void sleep(uint32 timer_count)
{
  wait_for_io(timer_count);
}

// get and read the keys  and kernel reaction
void input()
{
  char ch = 0;
  char keycode = 0;
  do{
    keycode = get_input_prompt();
    // if client press enter (KEY)
    if(keycode == KEY_ENTER){

      //for (size_t i = 0; i < count; i++) {

        newline_on_terminal();

      print_on_terminal("[ root @ root ]# ");

    }
    //if client press arrow up (KEY)
    else if(keycode == KEY_UP){
          newline_on_terminal();
          print_on_terminal("KEY UP PRESSED : ");
          newline_on_terminal();
          print_on_terminal("this key not work in this terminal !");
          newline_on_terminal();

          print_on_terminal("just type the command ! ");
          newline_on_terminal();

          print_on_terminal("[ root @ root ]# ");
          get_input_prompt();
    }
    //if client press arrow down (KEY)
    else if(keycode == KEY_DOWN){
          newline_on_terminal();
          print_on_terminal("KEY DOWN PRESSED : ");
          newline_on_terminal();
          print_on_terminal("this key not work in this terminal !");
          newline_on_terminal();

          print_on_terminal("just type the command ! ");
          newline_on_terminal();

          print_on_terminal("[ root @ root ]# ");
          get_input_prompt();
    }
    //if client press TAB (KEY)
    else if(keycode == KEY_TAB){
          newline_on_terminal();
          print_on_terminal("TAB PRESSED : ");
          newline_on_terminal();
          print_on_terminal("for shutdown : PWOEROFF");
          newline_on_terminal();
          print_on_terminal("for sleep : SLEEPOS");
          newline_on_terminal();

          print_on_terminal("[ root @ root ]# ");
          get_input_prompt();
    }
    //if client press ESC (KEY)
    else if(keycode == KEY_ESC){
        init_vga(RED,BLACK);
        newline_on_terminal();
        newline_on_terminal();
        newline_on_terminal();
        print_on_terminal("EXIT !");
        newline_on_terminal();
        print_on_terminal("if you wnat use CHARLEX-OS => please REBOOT your machine !");
        get_input_prompt();
        input();
    }

    else{
      ch = get_ascii_char(keycode);
      print_char(ch);
    }
    for (int i = 0; i < 1; i++){
      sleep(0x05FFFFFF);
    }

  }while(ch > 0);
}

// this function load logo

void logo(){
  print_on_terminal("      _                _     __  __");
  newline_on_terminal();

  print_on_terminal("  ___| |__   __ _ _ __| | ___\\\\ \\/ /");
  newline_on_terminal();

  print_on_terminal(" / __| '_  \\/ _` | '__| |/ _ \\\\  /");
  newline_on_terminal();

  print_on_terminal("| (__| | | | (_| | |  | |  __//  \\");
  newline_on_terminal();

  print_on_terminal(" \\___|_| |_|\\__,_|_|  |_| \\___/_/\\_\\");
  newline_on_terminal();

  print_on_terminal("        	  ___  ____      ");
  newline_on_terminal();

  print_on_terminal("        	 / _ \\/ ___|    ");
  newline_on_terminal();

  print_on_terminal("        	| | | \\___ \\   ");
  newline_on_terminal();

  print_on_terminal("        	| |_| |___) |    ");
  newline_on_terminal();

  print_on_terminal("        	 \\___/|____/    ");
}

// kernel entery point !!
void kernel_entry()
{


  // color of terminal(you can change it if you want :D )
  init_vga(GREEN, BLACK);
  newline_on_terminal();
  logo();


  newline_on_terminal();

  newline_on_terminal();

  print_on_terminal("|------------------|");
  newline_on_terminal();
  print_on_terminal("!Welcome to charleX!");
  newline_on_terminal();
  print_on_terminal("|------------------|");

  for (int i = 0; i < 40; i++){
    sleep(0x02FFFFFF); // sleep for logo to load ;;
  }
  //color set to green and black
  init_vga(GREEN , BLACK);
  newline_on_terminal();
  // input() for   infinity loop (terminal loop infinity)
  print_on_terminal("[ root @ root ]# ");
  input();

}
