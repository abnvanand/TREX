#include <iostream>

#include "util.h"
#include "cursor_control.h"
#include "normal_mode.h"
#include "command_mode.h"

using namespace std;


int main() {
    // TODO: find a better way
    system("tput smcup");        // switch to secondary screen

    normal_mode();

    system("tput rmcup");         // switch back to primary screen

    return 0;
}