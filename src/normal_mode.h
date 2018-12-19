/**
 * Name: Abhinav Anand
 * Roll Number: 2018201037
**/


#ifndef NORMAL_MODE_H
#define NORMAL_MODE_H

#include <iostream>
#include <termios.h>
#include <utility>
#include <cstdlib>
#include <stack>
#include "cursor_control.h"
#include "keymap.h"
#include "util.h"
#include "list.h"

int normal_mode();

int refresh_normal_mode(std::string &dir_name,
                        std::vector<std::string> &back_stack,
                        std::vector<std::string> &forward_stack);

#endif //NORMAL_MODE_H
