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

void join(const std::vector<std::string> &v, char c, std::string &s) {
    s.clear();
    for (auto p = v.begin(); p != v.end(); ++p) {
        s += *p;
        if (p != v.end() - 1)
            s += c;
    }
}

void build_home_path(const std::vector<std::string> &v, std::string &s) {
    s.clear();
    for (unsigned int i = 1; i < v.size(); i++) {
        s += "../";
    }
}
