#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <sys/stat.h>


int create_file(const std::string &filepath);

int copy_files_to_dir(const std::vector<std::string> &source_files, const std::string &dest);

int move_files_to_dir(const std::vector<std::string> &files, const std::string &dest);

int remove_dir(std::string dirname);

int search_dir(const std::string &dirname, const std::string &query, std::vector <std::string> &results);

#endif //COMMANDS_H
