#ifndef LIST_H
#define LIST_H

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <algorithm>
#include "util.h"


std::string get_permission_string(mode_t &st_mode);

void print_details(std::string i);

std::vector<std::string> ls(std::string dirname);

#endif //LIST_H
