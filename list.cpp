#include "list.h"

using namespace std;

//__int128_t YOTA = 1208925819614629174706176;
//__int128_t ZETTA = 1180591620717411303424;
//__off_t EXA = 1152921504606846976;
__off_t PETA = 1125899906842624;
__off_t TERRA = 1099511627776;
__off_t GIGA = 1073741824;
__off_t MEGA = 1048576;
__off_t KILO = 1024;
__off_t BYTE = 1;

/**
 *
 * @param size in Bytes
 * @return size in human readable format(KB, GB, TB)
 */
string human_readable_size(__off_t size) {
    string res;
    if (size > PETA) {
        res = to_string(size / PETA) + "PB";
    } else if (size > TERRA) {
        res = to_string(size / TERRA) + "TB";
    } else if (size > GIGA) {
        res = to_string(size / GIGA) + "GB";
    } else if (size > MEGA) {
        res = to_string(size / MEGA) + "MB";
    } else if (size > KILO) {
        res = to_string(size / KILO) + "KB";
    } else {
        res = to_string(size) + "B";
    }
    return res;
}

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
 * @param dirname
 */
vector<string> ls(string dirname) {
    DIR *dirptr;
    vector<string> list;

    struct dirent *item;
    // we need to use c_str() bcoz opendir accepts on char * and converting
    // our dirname to char * copies some invalid characters into it.
    if ((dirptr = opendir(dirname.c_str())) == nullptr) {
        cout << "Error opening directory: " << dirname << "\n";
        return list;
    }

    chdir(dirname.c_str());
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

    cout << setw(10) << get_permission_string(statbuf.st_mode);

    cout << setw(10) << getpwuid(statbuf.st_uid)->pw_name;
    cout << setw(10) << getgrgid(statbuf.st_gid)->gr_name;

    cout << setw(8) << human_readable_size(statbuf.st_size);

    cout << setw(18) << timestamp_to_localtime(statbuf.st_mtime);

    // Print name at the end bcoz it can have variable length
//    cout << setw(20) << name << (S_ISDIR(statbuf.st_mode) ? "/" : "");
    printf("    %.14s%s%s", name.c_str(), (name.length() > 14 ? "..." : ""), (S_ISDIR(statbuf.st_mode) ? "/" : ""));
}
