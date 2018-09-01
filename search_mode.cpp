/**
 * Name: Abhinav Anand
 * Roll Number: 2018201037
**/


#include "search_mode.h"
#include "commands.h"

using namespace std;

int search_mode(const string &query, string &extra_param) {
    vector<string> search_results;

    search_dir(pwd(), query, search_results);

    if (search_results.empty()) {
        cout << "No match found for: " << query << " " << "Press enter to continue.";
        getchar();
        return RES_CONTINUE;
    }

    clear_screen();
    string info_line = "SEARCH_PATH: " + pwd();
    draw_info_line(info_line);
    move_cursor_xy(1, 1);
    save_cursor_pos();  // save current cursor position it will point to first item in list

    unsigned int rows, cols;
    get_window_size(rows, cols);
    unsigned int max_num_rows = rows - 3;   // subtract 1 for command line 1 for status line and 1 for a blank line

    scroll_screen(1, max_num_rows);         // * Beware DONOT change this magic range [1,max_num_rows]


    unsigned int i = 0;
    for (; i < search_results.size() and i < max_num_rows - 1; i++) { // * Beware DONOT change i<max_num_rows
        if (i == 0) {
            underline_on();
            cout << search_results[i] << endl;
            underline_off();
        } else {
            cout << search_results[i] << endl;
        }
    }
    move_cursor_xy(1, 1);


    unsigned long current_row_number = 1;
    unsigned long current_index = 0;
    unsigned long last_index = search_results.size() - 1;

    char ch;

    while (true) {
        ch = (char) cin.get();

        if (ch == ARROW_UP) {
            if (current_index > 0) {
                save_cursor_pos();
                cout << search_results[current_index];
                restore_cursor_pos();
                current_index -= 1;
                scroll_up();
                save_cursor_pos();
                underline_on();
                cout << search_results[current_index];
                underline_off();
                restore_cursor_pos();

                if (current_row_number > 1)
                    current_row_number -= 1;
                write_row_number(current_row_number);
            }
        } else if (ch == ARROW_DOWN) {
            if (current_index < last_index) {
                save_cursor_pos();
                cout << search_results[current_index];
                restore_cursor_pos();
                current_index += 1;
                scroll_down();
                save_cursor_pos();
                underline_on();
                cout << search_results[current_index];
                underline_off();
                restore_cursor_pos();

                if (current_row_number < max_num_rows)
                    current_row_number += 1;

                write_row_number(current_row_number);
            }
        } else if (ch == ENTER) {
            extra_param = search_results[current_index];

            struct stat buf;
            lstat(extra_param.c_str(), &buf);
            if (S_ISDIR(buf.st_mode)) {
                return RES_GOTO_COMMAND;
            } else {
                // It's a file
                pid_t pid = fork();

                if (pid == 0) {
                    // child process
                    execl("/usr/bin/xdg-open", "xdg-open", extra_param.c_str(), (char *) nullptr);
                    exit(1);
                }

                // don't return if a file was opened
                // continue showing the search results
                continue;
            }

        } else if (ch == ESCAPE) {
            // ignore escape key bcoz it conflicts with arrow keys
            continue;
        } else if (ch == ':') {
            extra_param = DIRTY;
            return RES_CONTINUE;    // go back to command mode
        }
    }
}