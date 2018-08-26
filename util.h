#ifndef UTIL_H
#define UTIL_H

#include <ctime>
#include <string>

#define RES_QUIT 0
#define RES_NORMAL_MODE 1
#define RES_COMMAND_MODE 2
#define RES_ERROR 3
#define RES_GOTO_DIR 4

std::string timestamp_to_localtime(time_t timestamp);

#endif //UTIL_H
