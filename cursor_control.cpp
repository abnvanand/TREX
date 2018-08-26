#include "cursor_control.h"

using namespace std;


// READ https://stackoverflow.com/a/45067593/5463404
//      http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html

// FIXME :Clears the screen and moves the cursor to the line where command was entered
// should move the cursor to the top of the screen right now it isn't
void clear_screen() {
    cout << "\033[2J";
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

void clear_row(int row_number) {
    save_cursor_pos();
    move_cursor_xy(row_number, 1);
    cout << "\033[K";
    restore_cursor_pos();
}

void get_window_size(int &rows, int &cols) {
    struct winsize ws;
    ioctl(fileno(stdout), TIOCGWINSZ, &ws);
    cols = ws.ws_col;
    rows = ws.ws_row;
}


void draw_command_line() {
    // get window size
    int rows, cols;
    get_window_size(rows, cols);

    clear_row(rows);
    move_cursor_xy(rows, 1);

    cout << ":";
}

void draw_info_line(string current_mode) {
    save_cursor_pos();
    int rows, cols;
    get_window_size(rows, cols);
    clear_row(rows - 1);
    move_cursor_xy(rows - 1, 1);
    cout << current_mode;
    restore_cursor_pos();
}

void print_coloured_output(string text) {
    cout << "\033[1;31m" << text << "\033[0m";
    int size = (int) text.size() + 2;
    move_left(size);
}

void scroll_up() {
    cout << "\033M";
}

void scroll_down() {
    cout << "\033D";
}

void scroll_screen() {
    cout << "\033[r";
}