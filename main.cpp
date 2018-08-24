#include <iostream>
#include "list.h"
#include "cursor_control.h"
#include "normal_mode.h"

using namespace std;


int main() {
    clear_screen();

    cout << "--------------BEGIN-----------" << endl;
    ls(".");
    cout << "--------------END-----------" << endl;

    move_cursor_xy(0, 0);

    switch_to_normal_mode();

    return 0;
}