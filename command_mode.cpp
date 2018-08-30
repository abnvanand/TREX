#include "command_mode.h"
#include "commands.h"

using namespace std;

int command_mode(string HOME_PATH) {
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
                execute_command(command, HOME_PATH);
                command.clear();   // reset command
                draw_command_line();
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


void execute_command(string &command, const string &HOME_PATH) {
    const string ENTER_TO_CONTINUE = "Press enter to continue.";

    draw_command_line();
    vector<string> tokens = get_tokens(command);

    if (tokens.empty()) {   // though this should never happen :)
        cout << "Invalid command" << ENTER_TO_CONTINUE;
        getchar();  // Hold screen to show error
        return;
    }


    // :copy <source_file(s)> <destination_directory>
    if (tokens[0] == "copy") {
        if (tokens.size() < 3) {
            // Minimum size should be 3=> `copy` `source` `dest`
            cout << "Minimum 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return;
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

        return;
    }

    // :move <source_file(s)> <destination_directory>
    if (tokens[0] == "move") {
        if (tokens.size() < 3) {
            // Minimum size should be 3=> `copy` `source` `dest`
            cout << "Minimum 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return;
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
        return;
    }

    // :rename <old_filename> <new_filename>
    if (tokens[0] == "rename") {
        if (tokens.size() != 3) {
            cout << "Exactly 2 arguments required. " << ENTER_TO_CONTINUE;
            getchar();
            return;
        }

        if (rename(tokens[1].c_str(), tokens[2].c_str()) == -1) {
            cout << "Error renaming: " << tokens[1] << " " << ENTER_TO_CONTINUE;
            getchar();
            return;
        } else {
            cout << "Rename successful. " << ENTER_TO_CONTINUE;
            getchar();
            return;
        }
    }

    // :create_file foo.txt ~/foobar
    if (tokens[0] == "create_file") {
        if (tokens.size() != 3) {
            cout << "Exactly 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return;
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
        return;
    }


    // :create_dir folder_name ~/foobar
    if (tokens[0] == "create_dir") {
        if (tokens.size() != 3) {
            cout << "Exactly 2 arguments required." << ENTER_TO_CONTINUE;
            getchar();
            return;
        }

        int status = mkdir((tokens[2] + "/" + tokens[1]).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status == -1) {
            cout << "Error creating directory:" << tokens[2] << "/" << tokens[1];
            getchar();
            return;
        } else {
            cout << "Success. " << ENTER_TO_CONTINUE;
            getchar();
            return;
        }
    }

    // :delete_file <file_path>
    // NOTE: <file_path> is relative to APPLICATION root.
    if (tokens[0] == "delete_file") {
        if (tokens.size() != 2) {
            cout << "Too few arguments. " << ENTER_TO_CONTINUE;
            getchar();
            return;
        }
        if (remove((HOME_PATH + "/" + tokens[1]).c_str()) == -1) {
            cout << "Error deleting the file: " << tokens[1];
            getchar();
            return;
        }

        cout << "Deleted " << tokens[1] << " ." << ENTER_TO_CONTINUE;
        getchar();

        return;
    }

    // :delete_dir <directory_path>
    // NOTE: <file_path> is relative to APPLICATION root.
    if (tokens[0] == "delete_dir") {
        if (tokens.size() != 2) {
            cout << "Too few arguments. " << ENTER_TO_CONTINUE;
            getchar();
            return;
        }

        int res = remove_dir((HOME_PATH + "/" + tokens[1]));

        if (res == -1) {
            cout << "Error deleting dir: " << tokens[1] << " " << ENTER_TO_CONTINUE;
            getchar();
            return;
        }
        cout << "Deleted " << HOME_PATH << "/" << tokens[1] << " " << ENTER_TO_CONTINUE;
        getchar();
        return;
    }


    // If none matched then command is invalid
    cout << "Invalid command:" << tokens[0] << " " << ENTER_TO_CONTINUE;
    getchar();

    // TODO   goto <directory_path>
    // TODO   search <filename>
    // TODO   snapshot <folder> <dumpfile>
}
