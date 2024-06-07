#include "../includes/input_init.h"

// get and read the keys  and kernel reaction
void input(){
  char ch = 0;
  char keycode = 0;
  int enter_cuonter = 1;

  do{
    keycode = get_input_prompt();
    // if user press enter (KEY)
    if(keycode == KEY_ENTER){
      newline_on_terminal();

      print_on_terminal("# ");

      enter_cuonter--;
      ++enter_cuonter;

    }
    /*
     * if user press backspace for delete a single
     */
    else if(keycode == KEY_BACKSPACE && vga_index>82){
        if (vga_index >= 2){

          vga_index = vga_index-1;
          print_on_terminal(" ");
          vga_index = vga_index-1;
          
          

          
        }

        else{

          get_input_prompt();

        }        
        
    }

    //if user press arrow up (KEY
    else if(keycode == KEY_UP){
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("KEY : UP | STATUS: PRESSED  ; ");
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("# ");
          enter_cuonter++;
          get_input_prompt();
    }
    //if user press arrow down (KEY)
    else if(keycode == KEY_DOWN){
          print_on_terminal("KEY : DOWN | STATUS : PRESSED; ");
          print_on_terminal("# ");
          enter_cuonter++;
          get_input_prompt();
    }
    //if user press TAB (KEY)
    else if(keycode == KEY_TAB){
          newline_on_terminal();
          print_on_terminal("KEY : TAB | STATUS : PRESSED; ");
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("[ESC (key)] : exit, [BACKSPACE (key)] : delete a charecter ");
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("# ");
          enter_cuonter++;
          get_input_prompt();
    }
    //if user press ESC (KEY)
    else if(keycode == KEY_ESC){
        newline_on_terminal();
        newline_on_terminal();
        print_on_terminal("EXIT ! : ");
        newline_on_terminal();
        newline_on_terminal();
        print_on_terminal("HALT MODE : Halt the PC and disable keyboard?!");
        newline_on_terminal();
        newline_on_terminal();
        print_on_terminal("PLEASE CONFIRM WITH <ESC> AGAIN (HALT MODE)?"); 
        newline_on_terminal();
        newline_on_terminal();
        break;break;
    }

    else{
      ch = get_ascii_char(keycode);
      print_char(ch);


    }
    /*
     * NO SCROLL TERMINAL (static prompt )
     */
    if(enter_cuonter<5){
      enter_cuonter = enter_cuonter *2;
      next_line_index = next_line_index - enter_cuonter;
    }
    else{
      next_line_index = next_line_index - enter_cuonter;
    }


    for (int i = 0; i < 1; i++){
      sleep(0x4CFFFFF8);/* keyboard type speed for (((real machine))) */
    }

  }while(ch > 0);

}
