#include <iostream>

#include "util.h"
#include "cursor_control.h"
#include "normal_mode.h"
#include "command_mode.h"

using namespace std;


int main() {
    // TODO: find a better way
    system("tput smcup");        // switch to secondary screen

    while (true) {
        int res = normal_mode();

        if (res == RES_COMMAND_MODE) {
            command_mode();
        } else if (res == RES_NORMAL_MODE) {
            normal_mode();
        } else
            break;
    }

    system("tput rmcup");         // switch back to primary screen

    return 0;
}