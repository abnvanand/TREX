/**
 * Name: Abhinav Anand
 * Roll Number: 2018201037
**/


#ifndef TREX_SNAPSHOT_H
#define TREX_SNAPSHOT_H

#include <dirent.h>
#include <cstring>
#include <deque>
#include <algorithm>
#include <vector>
#include <fstream>

int create_snapshot(const std::string &dumpfile_name, const std::string &basedir);

int scandir_recursive(const std::string &dirname, std::ofstream &myfile);


#endif //TREX_SNAPSHOT_H
