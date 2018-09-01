/**
 * Name: Abhinav Anand
 * Roll Number: 2018201037
**/


#include "cursor_control.h"

using namespace std;


// READ https://stackoverflow.com/a/45067593/5463404
//      http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html

void clear_screen() {
    cout << "\033[2J";
//    cout << "\033[3J";           // Not needed if switching screen using tput smcup
//    cout << "\033[H\033[J";     // Not compatible with all terminals
}


void move_cursor_xy(int x, int y) {
    cout << "\033[" << x << ";" << y << "H";
}


void move_up(int steps) {
    cout << "\033[" << steps << "A";
}

void move_down(int steps) {
    cout << "\033[" << steps << "B";
}

void move_right(int steps) {
    cout << "\033[" << steps << "C";
}

void move_left(int steps) {
    cout << "\033[" << steps << "D";
}

void move_cursor(char direction) {
    cout << "\033[" << 1 << direction;
}

void save_cursor_pos() {
    cout << "\033[s";
}

void restore_cursor_pos() {
    cout << "\033[u";
}

void clear_row(unsigned int row_number) {
    save_cursor_pos();
    move_cursor_xy(row_number, 1);
    cout << "\033[K";
    restore_cursor_pos();
}

void get_window_size(unsigned int &rows, unsigned &cols) {
    struct winsize ws;
    ioctl(fileno(stdout), TIOCGWINSZ, &ws);
    cols = ws.ws_col;
    rows = ws.ws_row;
}


void draw_command_line() {
    // get window size
    unsigned int rows, cols;
    get_window_size(rows, cols);

    clear_row(rows);
    move_cursor_xy(rows, 1);

    cout << ":";
}

void draw_info_line(string current_mode) {
    save_cursor_pos();
    unsigned int rows, cols;
    get_window_size(rows, cols);
    clear_row(rows - 1);
    move_cursor_xy(rows - 1, 1);
    cout << current_mode;
    string dimension_string = to_string(rows) + "X" + to_string(cols);
    move_cursor_xy(rows - 1, static_cast<int>(cols - dimension_string.length()));
    cout << dimension_string;
    restore_cursor_pos();
}

// Writes the row number the cursor is pointing to in info line
void write_row_number(unsigned long row_num) {
    save_cursor_pos();
    unsigned int rows, cols;
    get_window_size(rows, cols);
    move_cursor_xy(rows - 2, cols - 3);
    if (row_num < 10)
        cout << 0 << row_num;
    else
        cout << row_num;
    restore_cursor_pos();
}

void print_coloured_output(string text) {
    cout << "\033[1;31m" << text << "\033[0m";
    int size = (int) text.size() + 2;
    move_left(size);
}

void underline_on() {
    cout << "\033[4m";
}

void underline_off() {
    cout << "\033[0m";
}

void scroll_up() {
    cout << "\033M";
}

void scroll_down() {
    cout << "\033D";
}

void scroll_screen(unsigned int start, unsigned int end) {
    cout << "\033[" + to_string(start) + ";" + to_string(end) + "r";
}