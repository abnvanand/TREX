#include "normal_mode.h"

using namespace std;

// We are in Normal mode (where you can't type anything exceptions are always there :)
/**
 * it's the responsibility of this module to leave the terminal in the state it was
 * @param initial_settings
 * @return
 */
int normal_mode() {
    struct termios initial_settings, new_settings;
    tcgetattr(fileno(stdin), &initial_settings);
    new_settings = initial_settings;

    int cause_of_return;

    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;         // to hide typed entry
    new_settings.c_lflag &= ~ISIG;         // to catch ctrl^c etc signal

    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;

    if (tcsetattr(fileno(stdin), TCSANOW, &new_settings) != 0) {
        cerr << "Could not set attributes\n";
        return RES_ERROR;
    }

    string dir = ".";
    vector<string> back_stack;
    vector<string> forward_stack;

    back_stack.emplace_back(".");

    while (true) {
        cause_of_return = refresh_normal_mode(dir, back_stack, forward_stack);

        if (cause_of_return != RES_GOTO_DIR)
            break;
    }

    // MUST restore the terminal to the way it was
    tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
    return cause_of_return;
}

int refresh_normal_mode(string &dir_name, vector<string> &back_stack, vector<string> &forward_stack) {
    clear_screen();
    string path;
    join(back_stack, '/', path);
    string info_line = "NORMAL_MODE: " + path;

    draw_info_line(info_line);
    move_cursor_xy(1, 1);

    save_cursor_pos();  // save current cursor position it will to first item in list

    vector<string> directory_list = ls(std::move(dir_name));

    /*
     */
    unsigned int rows, cols;
    get_window_size(rows, cols);
    unsigned int max_num_rows = rows - 3;   // subtract 1 for command line 1 for status line and 1 for a blank line

    scroll_screen(1, max_num_rows);         // * Beware DONOT change this magic range [1,max_num_rows] FIXME RCA?

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
    move_cursor_xy(1, 1);

    unsigned long current_index = 0;
    unsigned long last_index = directory_list.size() - 1;

    char ch;

    while (true) {
        ch = (char) cin.get();

        if (ch == ARROW_UP) {
            if (current_index > 0) {
                save_cursor_pos();
                print_details(directory_list[current_index]);
                restore_cursor_pos();
                current_index -= 1;
                scroll_up();
                save_cursor_pos();
                underline_on();
                print_details(directory_list[current_index]);
                underline_off();
                restore_cursor_pos();
            }
        } else if (ch == ARROW_DOWN) {
            if (current_index < last_index) {
                save_cursor_pos();
                print_details(directory_list[current_index]);
                restore_cursor_pos();
                current_index += 1;
                scroll_down();
                save_cursor_pos();
                underline_on();
                print_details(directory_list[current_index]);
                underline_off();
                restore_cursor_pos();
            }
        } else if (ch == ARROW_LEFT) {
            if (back_stack.size() == 1)
                continue;    // do nothing

            forward_stack.push_back(back_stack[back_stack.size() - 1]);
            back_stack.pop_back();
            dir_name = "..";
            return RES_GOTO_DIR;

        } else if (ch == ARROW_RIGHT) {
            if (forward_stack.empty())
                continue;   // do nothing

            dir_name = forward_stack[forward_stack.size() - 1];
            back_stack.push_back(dir_name);
            forward_stack.pop_back();
            return RES_GOTO_DIR;

        } else if (ch == BACKSPACE) {
            // Behaves just like '..'
            if (back_stack.size() == 1)
                continue; // do nothing

            forward_stack.push_back(back_stack[back_stack.size() - 1]);
            back_stack.pop_back();
            dir_name = "..";
            return RES_GOTO_DIR;

        } else if (ch == HOME) {
            if (back_stack.size() == 1)
                continue;

            build_home_path(back_stack, dir_name);
            back_stack.clear();
            forward_stack.clear();
            back_stack.emplace_back(".");
            return RES_GOTO_DIR;

        } else if (ch == ENTER) {
            //todo open dir/file pointed to by curremt_index
            dir_name = directory_list[current_index];

            if (dir_name == ".")
                // do nothing skip the loop
                continue;

            if (dir_name == "..") {
                if (back_stack.size() == 1)
                    continue; // do nothing

                forward_stack.push_back(back_stack[back_stack.size() - 1]);
                back_stack.pop_back();

            } else {
                // TODO: handle files separate from folders
                struct stat buf;
                lstat(dir_name.c_str(), &buf);
                if (S_ISDIR(buf.st_mode)) {
                    forward_stack.clear();
                    back_stack.push_back(dir_name);
                } else {
                    string command = "xdg-open " + dir_name;
                    // Need try catch to suppress GUI warnings.
                    // FIXME : warnings still visible on some systems
                    try {
                        system(command.c_str());
                        continue;
                    }
                    catch (int e) {
                        // ignore
                        continue;
                    }
                }
            }
            return RES_GOTO_DIR;

        } else if (ch == ETX) {
            // ETX (end of text) (Ctrl+C )
            cout << "\nType q to quit\n";
        } else if (ch == QUIT) {
            return RES_QUIT;
        } else if (ch == COLON) {
            return RES_COMMAND_MODE;
        } else {
            // cout << "\n You entered: " << ch << '\n';
        }
    }
}
