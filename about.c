#include "includes/kernel.h"
#include "includes/utils.h"
#include "includes/char.h"
#include "includes/reqs.h"


// kernel entery point !!
void kernel_up(){


  // color of terminal(you can change it if you want :D )
  init_vga(WHITE, BLACK);
  logo();
  print_on_terminal("Creator(s) : AMZYEI,");
  newline_on_terminal();
  newline_on_terminal();
}

