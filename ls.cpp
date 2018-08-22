#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <algorithm>

using namespace std;

struct FileObj {
    string name;
    struct stat statbuf;
};

/**
 * converts timestamp to localtime
 * @param timestamp
 * @return string representation of time
 */
string timestamp_to_localtime(time_t timestamp) {
    struct tm lt;
    localtime_r(&timestamp, &lt);
    char timbuf[80];

    // source https://stackoverflow.com/a/13542409/5463404
    strftime(timbuf, sizeof(timbuf), "%d.%m.%Y %H:%M", &lt);
    return timbuf;
}

// FIXME: sort should imitate ls in the order of results
int cmp(FileObj o1, FileObj o2) {
    return o1.name < o2.name;
}


/**
 * Behaves like the ls command of terminal.
 * @param dir
 */
void ls(char const *dir) {
    DIR *dirptr;
    vector<FileObj> list;

    struct dirent *item;

    if ((dirptr = opendir(dir)) == nullptr) {
        cout << "Error opening directory: " << dir << "\n";
        return;
    }

    while ((item = readdir(dirptr)) != nullptr) {
        FileObj obj;
        obj.name = item->d_name;

        lstat(item->d_name, &obj.statbuf);

        list.emplace_back(obj);
    }

    sort(list.begin(), list.end(), cmp);

    for (const auto &i: list) {
        // TODO: make human readable like ls -lh
        cout << setw(10) << i.statbuf.st_size << "";

        // TODO: test long user names
        cout << setw(10) << getpwuid(i.statbuf.st_uid)->pw_name << " ";
        cout << setw(10) << getgrgid(i.statbuf.st_gid)->gr_name << "";

        // TODO: Find how to get permission from mode read page 140 and 141
        cout << setw(10) << i.statbuf.st_mode << " ";

        cout << timestamp_to_localtime(i.statbuf.st_mtime) << " ";

        // Print name at the end bcoz it can have variable length
        cout << "\t" << i.name;
        cout << "\n";
    }

    closedir(dirptr);
}

int main() {
    cout << "--------------BEGIN-----------" << endl;
    ls(".");
    cout << "--------------END-----------" << endl;
    return 0;
}