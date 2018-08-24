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
