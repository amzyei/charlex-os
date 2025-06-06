/*
  Emulation for key board terminal-like bindings.
  Version 0.2
 */
#include "../include/input_init.h"
// get and read the keys and kernel reaction
/* Custom string compare function to avoid standard library dependency */
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

#define MAX_INPUT_LENGTH 64

void clear_screen() {
    // Clear the terminal screen by filling VGA buffer with spaces
    char* vga_buffer = (char*)0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        vga_buffer[i] = ' ';
        vga_buffer[i + 1] = 0x07; // light grey on black
    }
    vga_index = 82; // Reset cursor position after clear
}

void input() {
    char ch = 0;
    char keycode = 0;
    char input_buffer[MAX_INPUT_LENGTH];
    int input_pos = 0;

    // Print initial prompt
    print_on_terminal("# ");

    do {
        keycode = get_input_prompt();

        if (keycode == KEY_ENTER) {
            // Null-terminate input buffer
            input_buffer[input_pos] = '\0';

            // Check if command is "clear"
            if (my_strcmp(input_buffer, "clear") == 0) {
                clear_screen();
                print_on_terminal("# ");
            } else {
                newline_on_terminal();
                print_on_terminal("# ");
            }

            // Reset input buffer position
            input_pos = 0;
        }
        else if (keycode == KEY_BACKSPACE && input_pos > 0) {
            input_pos--;
            vga_index -= 2; // Move cursor back by 2 bytes (character + attribute)
            print_on_terminal("  "); // Overwrite character with space
            vga_index -= 2; // Move cursor back again
        }
        else if (keycode == KEY_UP) {
            newline_on_terminal();
            newline_on_terminal();
            print_on_terminal("KEY:UP | STATUS:PRESSED");
            newline_on_terminal();
            newline_on_terminal();
            print_on_terminal("# ");
        }
        else if (keycode == KEY_DOWN) {
            newline_on_terminal();
            print_on_terminal("KEY:DOWN | STATUS:PRESSED");
            newline_on_terminal();
            newline_on_terminal();
            print_on_terminal("# ");
            newline_on_terminal();
            newline_on_terminal();
        }
        else if (keycode == KEY_TAB) {
            // No action for TAB yet
        }
        else if (keycode == KEY_ESC) {
            print_on_terminal("EXIT ! : ");
            break;
        }
        else {
            ch = get_ascii_char(keycode);
            if (input_pos < MAX_INPUT_LENGTH - 1) {
                input_buffer[input_pos++] = ch;
                print_char(ch);
            }
        }

        // SCROLLING TERMINAL logic
        if (vga_index >= 80 * 25 * 2) {
            // Scroll screen up by one line (80*2 bytes)
            char* vga_buffer = (char*)0xB8000;
            for (int i = 0; i < (80 * 24 * 2); i++) {
                vga_buffer[i] = vga_buffer[i + 80 * 2];
            }
            // Clear last line
            for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
                vga_buffer[i] = ' ';
                vga_buffer[i + 1] = 0x07;
            }
            vga_index -= 80 * 2;
        }

        for (int i = 0; i < 1; i++) {
            sleep(0x4CFFFFF3); /* keyboard speed for (((real machine))) */
        }

    } while (ch > 0);
}
