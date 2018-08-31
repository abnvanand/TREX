#include <iostream>

#include "util.h"
#include "cursor_control.h"
#include "normal_mode.h"
#include "command_mode.h"

using namespace std;


int main() {
    // Switch screen so that we do not pollute the original terminal screen. (vi, htop etc like effect)
    // This can be safely removed. // If removing this uncomment code in cursor_control.clear_screen();
    system("tput smcup");        // switch to secondary screen

    normal_mode();

    system("tput rmcup");         // switch back to primary screen

    return 0;
}