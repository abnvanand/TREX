#ifndef UTIL_H
#define UTIL_H

#include <ctime>
#include <string>
#include <vector>

#define RES_QUIT 0
#define RES_NORMAL_MODE 1
#define RES_COMMAND_MODE 2
#define RES_ERROR 3
#define RES_GOTO_DIR 4

std::string timestamp_to_localtime(time_t timestamp);

void join(const std::vector<std::string> &v, char c, std::string &s);

void build_home_path(const std::vector<std::string> &v, std::string &s);

#endif //UTIL_H
