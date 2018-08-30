#ifndef COMMAND_MODE_H
#define COMMAND_MODE_H

#include <iostream>
#include <vector>

#include "keymap.h"
#include "cursor_control.h"
#include "util.h"

#define ENTER_TO_CONTINUE "Press enter to continue."

int command_mode(std::string HOME_PATH, std::string &extra_param);

int execute_command(std::string &command, const std::string &HOME_PATH, std::string &extra_param);


#endif //COMMAND_MODE_H