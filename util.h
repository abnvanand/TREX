#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <unistd.h>

#define RES_QUIT 0
#define RES_NORMAL_MODE 1
#define RES_COMMAND_MODE 2
#define RES_ERROR 3
#define RES_ENTERED_DIR 4


#define RES_SEARCH_COMMAND 5
#define RES_GOTO_COMMAND 6
#define RES_CONTINUE 7

#define DIRTY "dirty"

std::string timestamp_to_localtime(time_t timestamp);

std::string pwd();

bool dir_exists(const std::string &path);

std::string get_proper_path(const std::string &path, const std::string &HOME_PATH);

#endif //UTIL_H
