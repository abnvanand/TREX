#include <iostream>
#include "list.h"
#include "cursor_control.h"
#include "normal_mode.h"

using namespace std;


int main() {
    // TODO: find a better way
    system("tput smcup");        // switch to secondary screen

    clear_screen();

    cout << "--------------BEGIN-----------" << endl;
    ls(".");
    cout << "--------------END-----------" << endl;

    move_cursor_xy(0, 0);

    switch_to_normal_mode();

    system("tput rmcup");         // switch back to primary screen

    return 0;
}