#include "list.h"

using namespace std;

// FIXME: sort should imitate ls in the order of results
int cmp(const string &a, const string &b) {
    return a < b;
}


// TODO: Find how to handle all possible file types. read page 140 and 141
// currently it handles only 'd' and '-' no 'l' for symlink etc..
void print_permissions(mode_t &st_mode) {
    cout << ((S_ISDIR(st_mode)) ? "d" : "-");
    cout << ((st_mode & S_IRUSR) ? "r" : "-");
    cout << ((st_mode & S_IWUSR) ? "w" : "-");
    cout << ((st_mode & S_IXUSR) ? "x" : "-");
    cout << ((st_mode & S_IRGRP) ? "r" : "-");
    cout << ((st_mode & S_IWGRP) ? "w" : "-");
    cout << ((st_mode & S_IXGRP) ? "x" : "-");
    cout << ((st_mode & S_IROTH) ? "r" : "-");
    cout << ((st_mode & S_IWOTH) ? "w" : "-");
    cout << ((st_mode & S_IXOTH) ? "x" : "-");
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

    // TODO: play with setw to find correct with
    // TODO: make human readable like ls -lh
    cout << setw(10) << statbuf.st_size;

    // TODO: test long user names
    cout << setw(10) << getpwuid(statbuf.st_uid)->pw_name;
    cout << setw(10) << getgrgid(statbuf.st_gid)->gr_name;

    cout << setw(8);
    print_permissions(statbuf.st_mode);

    cout << setw(18) << timestamp_to_localtime(statbuf.st_mtime);

    // Print name at the end bcoz it can have variable length
    cout << "  " << name << (S_ISDIR(statbuf.st_mode) ? "/" : "");
    cout << "\n";
}
