#include "command_mode.h"
#include "commands.h"
#include "search_mode.h"
#include "snapshot.h"

using namespace std;

int command_mode(string HOME_PATH, string &extra_param) {
    draw_info_line("COMMAND MODE");
    draw_command_line();

    string command;
    char ch;

    while (true) {
        // can't do cin>>ch READ: https://stackoverflow.com/questions/42818899/detecting-enter-key-in-c
        ch = (char) cin.get();

        if (ch == ESCAPE) {
            return RES_NORMAL_MODE;
        }

/*
 *      if (ch == ARROW_UP || ch == ARROW_DOWN) {
 *          // This if will never get executed.
 *          // RCA:=> arrow keys consist of multiple characters
 *          // and the first character is ESC
 *          // view README.md for more info.
 *          // Also we should NOT do this as
 *          // the user will then not be able to type A and B in caps
 *          continue;   // ignore and skip the loop
 *      }
 */

        if (ch == BACKSPACE) {
            if (command.length() > 0) {
                cout << "\b \b";
                command.pop_back();
            }

            continue;   // skip rest of the loop
        }


        if (ch == ENTER) {
            if (command.length() > 0) {
                int res = execute_command(command, HOME_PATH, extra_param);
                command.clear();   // reset command
                draw_command_line();

                // Return result codes for goto and search
                if (res == RES_GOTO_COMMAND) {
                    cout << "Moving to directory: " << extra_param << " ." << ENTER_TO_CONTINUE;
                    getchar();
                    return RES_GOTO_COMMAND;
                } else if (res == RES_NORMAL_MODE)
                    return RES_NORMAL_MODE;
            }
        } else {
            command += ch;
            cout << ch;
        }

    }
}

vector<string> get_tokens(string command) {
    vector<string> tokens;
    string token;

    for (char c: command) {
        // build the token until we encounter a space
        if (c == ' ') {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(token);    // push last token

    return tokens;
}


/**
 * Executes all command mode commands
 * Except :goto and :search.
 * :goto and :search are handled by normal mode.
 * command mode just passes the required parameters to perform these commands.
 * eg goto_path and search_query
 * @param command
 * @param HOME_PATH
 * @param extra_param => to be used for goto and search
 * @return Result code
 * and set extra_param with goto_path or search_query
 * Possible result codes are:-
 * RES_CONTINUE = continue looping
 * RES_GOTO_COMMAND
 * RES_SEARCH
 */
int execute_command(string &command, const string &HOME_PATH, string &extra_param) {

    draw_command_line();
    vector<string> tokens = get_tokens(command);

    if (tokens.empty()) {   // though this should never happen :)
        cout << "Invalid command" << ENTER_TO_CONTINUE;
        getchar();  // Hold screen to show error
        return RES_CONTINUE;
    }


    // :copy <source_file(s)> <destination_directory>
    if (tokens[0] == "copy") {
        if (tokens.size() < 3) {
            // Minimum size should be 3=> `copy` `source` `dest`
            cout << "Minimum 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
        vector<string> files;
        auto it = tokens.begin() + 1;
        while (it != tokens.end() - 1) {
            files.push_back(*it);
            it++;
        }
        copy_files_to_dir(files, tokens.back());

        // TODO : implement directory copying
        // https://stackoverflow.com/a/45546054/5463404

        return RES_CONTINUE;
    }

    // :move <source_file(s)> <destination_directory>
    if (tokens[0] == "move") {
        if (tokens.size() < 3) {
            // Minimum size should be 3=> `copy` `source` `dest`
            cout << "Minimum 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
        vector<string> files;
        auto it = tokens.begin() + 1;
        while (it != tokens.end() - 1) {
            files.push_back(*it);
            it++;
        }
        int res = move_files_to_dir(files, tokens.back());
        if (res == -1)
            cout << "Error moving files to: " << tokens.back() << ". " << ENTER_TO_CONTINUE;
        else
            cout << "Files moved successfully. " << ENTER_TO_CONTINUE;
        getchar();
        return RES_CONTINUE;
    }

    // :rename <old_filename> <new_filename>
    if (tokens[0] == "rename") {
        if (tokens.size() != 3) {
            cout << "Exactly 2 arguments required. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }

        if (rename(tokens[1].c_str(), tokens[2].c_str()) == -1) {
            cout << "Error renaming: " << tokens[1] << " " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        } else {
            cout << "Rename successful. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
    }

    // :create_file foo.txt ~/foobar
    if (tokens[0] == "create_file") {
        if (tokens.size() != 3) {
            cout << "Exactly 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }

        int fd;
        const char *filename;
        if (tokens[2] == ".") { // Create in current dir
            filename = tokens[1].c_str();
        } else {
            filename = (tokens[2] + "/" + tokens[1]).c_str();
        }

        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        if ((fd = creat(filename, mode)) < 0) {
            cout << "Error creating file. " << ENTER_TO_CONTINUE;
        } else {
            close(fd);
            cout << "Success. " << ENTER_TO_CONTINUE;
        }
        getchar();
        return RES_CONTINUE;
    }


    // :create_dir folder_name ~/foobar
    if (tokens[0] == "create_dir") {
        if (tokens.size() != 3) {
            cout << "Exactly 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }

        int status = mkdir((tokens[2] + "/" + tokens[1]).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status == -1) {
            cout << "Error creating directory:" << tokens[2] << "/" << tokens[1];
            getchar();
            return RES_CONTINUE;
        } else {
            cout << "Success. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
    }

    // :delete_file <file_path>
    // NOTE: <file_path> is relative to APPLICATION root.
    if (tokens[0] == "delete_file") {
        if (tokens.size() != 2) {
            cout << "Too few arguments. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
        if (remove((HOME_PATH + "/" + tokens[1]).c_str()) == -1) {
            cout << "Error deleting the file: " << tokens[1];
            getchar();
            return RES_CONTINUE;
        }

        cout << "Deleted " << tokens[1] << " ." << ENTER_TO_CONTINUE;
        getchar();

        return RES_CONTINUE;
    }

    // :delete_dir <directory_path>
    // NOTE: <file_path> is relative to APPLICATION root.
    if (tokens[0] == "delete_dir") {
        if (tokens.size() != 2) {
            cout << "Too few arguments. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }

        int res = remove_dir((HOME_PATH + "/" + tokens[1]));

        if (res == -1) {
            cout << "Error deleting dir: " << tokens[1] << " " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
        cout << "Deleted " << HOME_PATH << "/" << tokens[1] << " " << ENTER_TO_CONTINUE;
        getchar();
        return RES_CONTINUE;
    }

    // :goto <directory_path>
    // <directory_path> is relative to APP_ROOT
    // If / is given as directory path then goto APP_ROOT
    if (tokens[0] == "goto") {
        if (tokens.size() != 2) {
            cout << "Exactly one argument required. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
        string goto_path;

        if (tokens[1] == "/" || tokens[1] == "~") {
            // goto APP_ROOT aka HOME_PATH
            goto_path = HOME_PATH;
        } else if (tokens[1][0] == '/') {  // if the given path starts with ~ or /
            // APP ROOT relative path
            goto_path = HOME_PATH + "/" + tokens[1].substr(1);  // substr(1) == from index 1 till the end
        } else if (tokens[1][0] == '~' and tokens[1][1] == '/') {
            goto_path = HOME_PATH + "/" + tokens[1].substr(2);  // substr(2) means skip ~/ in ~/foobar
        } else {
            // currrent directory relative path
            goto_path = tokens[1];
        }

        // check if the path is valid
        if (!dir_exists(goto_path)) {
            cout << "Invalid path:" << goto_path << ". " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }

        extra_param = goto_path;
        return RES_GOTO_COMMAND;
    }

    // :search <filename>
    if (tokens[0] == "search") {
        if (tokens.size() != 2) {
            cout << "Exactly one argument required. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }
        int res = search_mode(tokens[1], extra_param);
        if (res == RES_GOTO_COMMAND) {
            // search mode sets extra_param to
            // absolute directory path of search result
            // treat it just like :goto command
            return RES_GOTO_COMMAND;
        } else if (res == RES_NORMAL_MODE) {
            return RES_NORMAL_MODE;
        }
        // else the result code is RES_CONTINUE
        draw_command_line();
        draw_info_line("COMMAND MODE");
        return RES_CONTINUE;
    }

    // :snapshot <folder> <dumpfile>
    if (tokens[0] == "snapshot") {
        if (tokens.size() != 3) {
            cout << "Exactly two arguments required. " << ENTER_TO_CONTINUE;
            getchar();
            return RES_CONTINUE;
        }

        int res = create_snapshot(tokens[2], tokens[1]);
        if (res == -1) {
            cout << "snapshot creation failed. " << ENTER_TO_CONTINUE;
        } else {
            cout << "snapshot created. " << ENTER_TO_CONTINUE;
        }

        getchar();
        return RES_CONTINUE;
    }

    // If none matched then command is invalid
    cout << "Invalid command:" << tokens[0] << " " << ENTER_TO_CONTINUE;
    getchar();
    return RES_CONTINUE;
}
