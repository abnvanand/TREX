#ifndef LIST_H
#define LIST_H

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <algorithm>

void print_permissions(mode_t st_mode);

void ls(char const *dir);

#endif //LIST_H
