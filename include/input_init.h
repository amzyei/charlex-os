#include "kernel.h"
#include "utils.h"
#include "char.h"

uint32 vga_index;
static uint32 next_line_index = 1;
uint8 g_fore_color = WHITE, g_back_color = BLACK;
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

extern uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color);
extern void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color);
extern void init_vga(uint8 fore_color, uint8 back_color);
extern void newline_on_terminal();
extern void print_char(char ch);
extern void print_on_terminal(char *str);
extern void print_int(int num);
extern uint8 inb(uint16 port);
extern void outb(uint16 port, uint8 data);
extern void wait_for_io(uint32 timer_count);
extern void sleep(uint32 timer_count);
extern void input();
extern void kernel_up();

uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color){
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

void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color){
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}


// set terminal color

void init_vga(uint8 fore_color, uint8 back_color){
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}


//this function print a new line on your termninal like '\n'

void newline_on_terminal(){
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}

void print_char(char ch){
  vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index++;
}

void print_on_terminal(char *str){
  uint32 index =0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

void print_int(int num){
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_on_terminal(str_num);
}

uint8 inb(uint16 port){
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}

void outb(uint16 port, uint8 data){
  asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}
// get keys from your keyboard
//and read
extern char get_input_prompt(){
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
void wait_for_io(uint32 timer_count){
  while(1){
    asm volatile("nop" /*do noting*/  );
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}
//sleep function
void sleep(uint32 timer_count){
  wait_for_io(timer_count);
}