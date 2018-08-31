#include "snapshot.h"

using namespace std;


int create_snapshot(const string &dumpfile_name, const string &basedir) {
    ofstream handle;
    handle.open(dumpfile_name.c_str());

    int res = scandir_recursive(basedir, handle);

    handle.close();
    return res;
}

int scandir_recursive(const string &dirname, ofstream &myfile) {
    deque<string> Q;
    vector<string> list;
    DIR *dp;
    struct dirent *entry;

    if ((dp = opendir(dirname.c_str())) == nullptr)
        return -1;

    myfile << dirname << ":" << endl;
    while ((entry = readdir(dp)) != nullptr) {
        // ignore . and ..
        if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0)
            continue;

        if (entry->d_type == DT_DIR) {
            string path = dirname + "/" + entry->d_name;
            Q.push_back(path);
        }
        list.emplace_back(entry->d_name);
    }

    sort(list.begin(), list.end());

    for (const auto &i: list) {
        myfile << i << "\t";
    }
    myfile << endl << endl;   // two new lines after contents of current dir have been printed

    sort(Q.begin(), Q.end());

    while (!Q.empty()) {
        string d = Q.front();
        Q.pop_front();
        scandir_recursive(d, myfile);
    }

    closedir(dp);
    return 0;
}
