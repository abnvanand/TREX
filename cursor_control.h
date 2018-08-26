#ifndef CURSOR_CONTROL_H
#define CURSOR_CONTROL_H

#include <iostream>
#include <sys/ioctl.h>

#include "keymap.h"

void clear_screen();

void move_cursor_xy(int x, int y);

void move_up(int steps);

void move_down(int steps);

void move_left(int steps);

void move_right(int steps);

/**
 * Moves cursor 1 step in supplied direction
 * @param direction
 */
void move_cursor(char direction);

void save_cursor_pos();

void restore_cursor_pos();

void clear_row(unsigned int row_number);

void get_window_size(unsigned int &rows, unsigned int &cols);

void draw_command_line();

void draw_info_line(std::string current_mode);

void print_coloured_output(std::string text);

void print_underlined_text(const std::string &text);

void execute_command(std::string &command);

void scroll_up();

void scroll_down();

void scroll_screen();

#endif //CURSOR_CONTROL_H
