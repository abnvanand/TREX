#include "command_mode.h"
#include "commands.h"
#include "search_mode.h"
#include "snapshot.h"
#include "list.h"

using namespace std;


void refresh_normal_mode_ui() {
    vector<string> directory_list = ls(".");

    clear_screen();

    draw_info_line("COMMAND MODE");
    draw_command_line();

    move_cursor_xy(1, 1);

//    save_cursor_pos();  // save current cursor position it will to first item in list


    unsigned int rows, cols;
    get_window_size(rows, cols);
    unsigned int max_num_rows = rows - 3;   // subtract 1 for command line 1 for status line and 1 for a blank line

//    scroll_screen(1, max_num_rows);         // * Beware DONOT change this magic range [1,max_num_rows] FIXME RCA?

    unsigned int i = 0;
    for (; i < directory_list.size() and i < max_num_rows - 1; i++) { // * Beware DONOT change i<max_num_rows FIXME RCA?
        if (i == 0) {
            underline_on();
            print_details(directory_list[i]);
            underline_off();
        } else {
            print_details(directory_list[i]);
        }
        cout << "\n";
    }
//    move_cursor_xy(1, 1);

}

void fire_refresh(string &extra_param) {
    save_cursor_pos();
    refresh_normal_mode_ui();
    restore_cursor_pos();
    extra_param = DIRTY;
}

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
    vector<string> tokens = escaped_tokenizer(command);

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
            files.push_back(get_proper_path(*it, HOME_PATH));
            it++;
        }
        int res = copy_files_to_dir(files, get_proper_path(tokens.back(), HOME_PATH));
        if (res <= -1)
            cout << "Error copying files to: " << get_proper_path(tokens.back(), HOME_PATH) << " " << ENTER_TO_CONTINUE;
        else {
            fire_refresh(extra_param);
            cout << "Files copied successfully. " << ENTER_TO_CONTINUE;
        }
        getchar();
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
            files.push_back(get_proper_path(*it, HOME_PATH));
            it++;
        }
        int res = move_files_to_dir(files, get_proper_path(tokens.back(), HOME_PATH));
        if (res == -1)
            cout << "Error moving files to: " << tokens.back() << ". " << ENTER_TO_CONTINUE;
        else {
            fire_refresh(extra_param);
            cout << "Files moved successfully. " << ENTER_TO_CONTINUE;
        }
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
            fire_refresh(extra_param);
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

        string filepath;
        if (tokens[2] == ".") { // Create in current dir
            filepath = tokens[1];
        } else {
            filepath = get_proper_path(tokens[2], HOME_PATH) + "/" + tokens[1];
        }

        if (create_file(filepath) == -1) {
            cout << "Error creating file. " << ENTER_TO_CONTINUE;
        } else {
            fire_refresh(extra_param);
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

        int status = mkdir((get_proper_path(tokens[2], HOME_PATH) + "/" + tokens[1]).c_str(),
                           S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status == -1) {
            cout << "Error creating directory:" << tokens[2] << "/" << tokens[1];
            getchar();
            return RES_CONTINUE;
        } else {
            fire_refresh(extra_param);
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

        fire_refresh(extra_param);
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
        fire_refresh(extra_param);
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

        goto_path = get_proper_path(tokens[1], HOME_PATH);

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
