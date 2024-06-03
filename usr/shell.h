#include "../includes/input_init.h"

// get and read the keys  and kernel reaction
void input(){
  char ch = 0;
  char keycode = 0;
  do{
    int enter_cuonter=1;
    keycode = get_input_prompt();
    // if client press enter (KEY)
    if(keycode == KEY_ENTER){
      newline_on_terminal();

      print_on_terminal("# ");

      enter_cuonter++;

    }
    /*
     * if client press backspace for delete a single
     */
    else if(keycode == KEY_BACKSPACE){
        if (vga_index > 0){

          vga_index = vga_index-1;
          print_on_terminal(" ");
          vga_index = vga_index-1;
          
        }

        else{

          get_input_prompt();

        }        
        
    }

    //if client press arrow up (KEY)
    else if(keycode == KEY_UP){
          newline_on_terminal();
          print_on_terminal("KEY : UP ARROW | STATUS: PRESSED  ; ");
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("# ");
          enter_cuonter++;
          get_input_prompt();
    }
    //if client press arrow down (KEY)
    else if(keycode == KEY_DOWN){
          newline_on_terminal();
          print_on_terminal("KEY : DOWN ARROW | STATUS : PRESSED; ");
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("# ");
          enter_cuonter++;
          get_input_prompt();
    }
    //if client press TAB (KEY)
    else if(keycode == KEY_TAB){
          newline_on_terminal();
          print_on_terminal("KEY : TAB | STATUS : PRESSED      ; ");
          newline_on_terminal();
          print_on_terminal("[ESC (key)] : exit, [BACKSPACE (key)] : delete a charecter ");
          newline_on_terminal();
          newline_on_terminal();
          print_on_terminal("# ");
          enter_cuonter++;
          get_input_prompt();
    }
    //if client press ESC (KEY)
    else if(keycode == KEY_ESC){
        init_vga(RED,BLACK);
        newline_on_terminal();
        print_on_terminal("EXIT ! : ");
        newline_on_terminal();
        print_on_terminal("HALT MODE : please REBOOT your machine !");
        newline_on_terminal();
        newline_on_terminal();
        break;
        enter_cuonter++;
        newline_on_terminal();
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
      sleep(0x4CFFFFFA);/* keyboard type speed for (((real machine))) */
    }

  }while(ch > 0);

}
