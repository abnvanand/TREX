#include "list.h"

using namespace std;

// FIXME: sort should imitate ls in the order of results
int cmp(const string &a, const string &b) {
    return a < b;
}


// To handle all possible file types. read page 140 and 141
// https://linux.die.net/man/2/stat
string get_permission_string(mode_t &st_mode) {
    string res;
    if (S_ISREG(st_mode)) {
        res += "-";
    } else if (S_ISDIR(st_mode)) {
        // directory
        res += "d";
    } else if (S_ISCHR(st_mode)) {
        // character device
        res += "c";
    } else if (S_ISBLK(st_mode)) {
        // block device
        res += "b";
    } else if (S_ISFIFO(st_mode)) {
        // FIFO(named pipe)
        res += "p";
    }

    // Don't use else if here bcoz symbolic link matches Regular file as well.
    if (S_ISLNK(st_mode)) {
        // symbolic link (Not in POSIX .1 - 1996.)
        res += "l";
    } else if (S_ISSOCK(st_mode)) {
        // socket (Not in POSIX .1 - 1996.)
        res += "s";
    }

    res += ((st_mode & S_IRUSR) ? "r" : "-");
    res += ((st_mode & S_IWUSR) ? "w" : "-");
    res += ((st_mode & S_IXUSR) ? "x" : "-");
    res += ((st_mode & S_IRGRP) ? "r" : "-");
    res += ((st_mode & S_IWGRP) ? "w" : "-");
    res += ((st_mode & S_IXGRP) ? "x" : "-");
    res += ((st_mode & S_IROTH) ? "r" : "-");
    res += ((st_mode & S_IWOTH) ? "w" : "-");
    res += ((st_mode & S_IXOTH) ? "x" : "-");
    return res;
}


/**
 * Behaves like the ls command of terminal.
 * @param dir
 */
vector<string> ls(string dir) {
    DIR *dirptr;
    vector<string> list;

    struct dirent *item;
    // we need to use c_str() bcoz opendir accepts on char * and converting
    // our name to char * copies some invalid characters into it.
    if ((dirptr = opendir(dir.c_str())) == nullptr) {
        cout << "Error opening directory: " << dir << "\n";
        return list;
    }

    chdir(dir.c_str());
    while ((item = readdir(dirptr)) != nullptr) {
        string name = item->d_name;
        list.emplace_back(name);
    }

    sort(list.begin(), list.end(), cmp);

    closedir(dirptr);
    return list;
}


void print_details(string name) {
    struct stat statbuf{};

    lstat(name.c_str(), &statbuf);

    // TODO: make human readable like ls -lh
    cout << setw(8) << statbuf.st_size;

    // TODO: test long user names
    cout << setw(10) << getpwuid(statbuf.st_uid)->pw_name;
    cout << setw(10) << getgrgid(statbuf.st_gid)->gr_name;

    cout << setw(12) << get_permission_string(statbuf.st_mode);

    cout << setw(18) << timestamp_to_localtime(statbuf.st_mtime);

    // Print name at the end bcoz it can have variable length
//    cout << setw(20) << name << (S_ISDIR(statbuf.st_mode) ? "/" : "");
    printf("    %.14s%s%s", name.c_str(), (name.length() > 14 ? "..." : ""), (S_ISDIR(statbuf.st_mode) ? "/" : ""));
}
