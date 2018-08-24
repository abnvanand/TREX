#ifndef CURSOR_CONTROL_H
#define CURSOR_CONTROL_H

#include <iostream>

void clear_screen();

void move_cursor_xy(int x, int y);

void move_up(int steps);

void move_down(int steps);

void move_left(int steps);

void move_right(int steps);

#endif //CURSOR_CONTROL_H
