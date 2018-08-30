#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <sys/stat.h>


void copy_files_to_dir(const std::vector<std::string> &files, const std::string &dest);

int move_files_to_dir(const std::vector<std::string> &files, const std::string &dest);

int remove_dir(std::string dirname);

#endif //COMMANDS_H
