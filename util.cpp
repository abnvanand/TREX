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