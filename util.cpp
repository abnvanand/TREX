#include <dirent.h>
#include "util.h"

/**
 * converts timestamp to localtime
 * @param timestamp
 * @return string representation of time
 */
std::string timestamp_to_localtime(time_t timestamp) {
    struct tm lt;
    localtime_r(&timestamp, &lt);
    char timbuf[80];

    // source https://stackoverflow.com/a/13542409/5463404
    strftime(timbuf, sizeof(timbuf), "%d.%m.%Y %H:%M", &lt);
    return timbuf;
}

std::string pwd() {
    char *cwd = get_current_dir_name();
    return cwd;
}

bool dir_exists(const std::string &path) {
    DIR *dirptr;

    // we need to use c_str() bcoz opendir accepts on char * and converting
    // our name to char * copies some invalid characters into it.
    if ((dirptr = opendir(path.c_str())) == nullptr) {
        return false;
    }

    closedir(dirptr);
    return true;
}