#ifndef COMMAND_MODE_H
#define COMMAND_MODE_H

#include <iostream>
#include <vector>

#include "keymap.h"
#include "cursor_control.h"
#include "util.h"

int command_mode(std::string HOME_PATH);

void execute_command(std::string &command,const std::string &HOME_PATH);


#endif //COMMAND_MODE_H