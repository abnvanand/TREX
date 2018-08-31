#include <dirent.h>
#include <cstring>
#include "commands.h"

#define DEFAULT_FILE_PERMS 0644
using namespace std;

void copy_perms(const char *fromFile, const char *toFile) {
    struct stat tmp;
    lstat(fromFile, &tmp);
    chmod(toFile, tmp.st_mode);
}

int copy_file(const string &from, const string &to) {
    char buf[BUFSIZ];
    ssize_t size;

    int source = open(from.c_str(), O_RDONLY, 0);
    if (source == -1) {
        return -1;
    }

    int dest = open(to.c_str(), O_WRONLY | O_CREAT, DEFAULT_FILE_PERMS);
    if (dest == -1) {
        return -2;
    }

    while ((size = read(source, buf, BUFSIZ)) > 0) {
        if (write(dest, buf, size) == -1)
            return -3;
        copy_perms(from.c_str(), to.c_str());
    }

    close(source);
    close(dest);
    return 0;
}

/**
 *
 * @param fullfilepath
 * @return the last part of the path which is acc=tually the filename
 */
string getfilename_from_path(const string &fullfilepath) {
    string token;

    for (char c: fullfilepath) {
        // build the token until we encounter a space
        if (c == '/') {
            token.clear();
        } else {
            token += c;
        }
    }

    return token;
}

// Assume that the destination directory exists and you have write
// permissions.
int copy_files_to_dir(const vector<string> &source_files, const string &dest) {
    for (const string &source_path :source_files) {
        string dest_file_path = dest + "/" + getfilename_from_path(source_path);
        if (copy_file(source_path, dest_file_path) == -1) {
            return -1;
        }
    }

    return 0;
}

int move_files_to_dir(const vector<string> &files, const string &dest) {
    string dest_dir;
    if (dest == ".")
        dest_dir = "./";
    else
        dest_dir = dest + "/";
    for (const string &i :files) {
        int res = rename(i.c_str(), (dest_dir + i).c_str());

        if (res == -1)
            return -1;
    }

    return 0;
}

int remove_dir(string dirname) {
    DIR *dir = opendir(dirname.c_str());

    if (dir == nullptr) {
//        cerr << "Error opendir()";    // Suppress output here
        return -1;
    }

    // First we recursively delete directory contents.
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        string path = dirname + "/" + entry->d_name;
        if (entry->d_type == DT_DIR) {
            remove_dir(path);
        }

        remove(path.c_str());
    }

    closedir(dir);

    // Delete the original directory now that it's empty
    remove(dirname.c_str());

    return 0;
}

int search_dir(const string &dirname, const string &query, vector<string> &results) {
    DIR *dp;

    struct dirent *entry;

    if ((dp = opendir(dirname.c_str())) == nullptr) {
        return -1;
    }

    while ((entry = readdir(dp)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        string filename = entry->d_name;
        string relative_path = dirname + "/" + entry->d_name;

        // Complete match
        if (filename == query) {
            string current_dir = get_current_dir_name();
            results.emplace_back(relative_path);
        }

        // Partial match
//        if(filename.find(query) != string::npos)
//            results.push_back(filename);

        if (entry->d_type == DT_DIR) {  // if it is a directory
            // Recursively go to sub directories
            search_dir(relative_path, query, results);
        }
    }

    closedir(dp);
    return 0;
}

