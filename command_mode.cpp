#include "command_mode.h"
#include "commands.h"

using namespace std;

int command_mode(string HOME_PATH) {
    draw_info_line("COMMAND MODE");
    draw_command_line();

    string command;
    char ch;

    while (true) {
        // can't do cin>>ch READ: https://stackoverflow.com/questions/42818899/detecting-enter-key-in-c
        ch = (char) cin.get();

        if (ch == ESCAPE) {
            return RES_NORMAL_MODE;
        }

/*
 *      if (ch == ARROW_UP || ch == ARROW_DOWN) {
 *          // This if will never get executed.
 *          // RCA:=> arrow keys consist of multiple characters
 *          // and the first character is ESC
 *          // view README.md for more info.
 *          // Also we should NOT do this as
 *          // the user will then not be able to type A and B in caps
 *          continue;   // ignore and skip the loop
 *      }
 */

        if (ch == BACKSPACE) {
            if (command.length() > 0) {
                cout << "\b \b";
                command.pop_back();
            }

            continue;   // skip rest of the loop
        }


        if (ch == ENTER) {
            if (command.length() > 0) {
            // TODO execute_command(command, HOME_PATH);
                command.clear();   // reset command
                draw_command_line();
            }
        } else {
            command += ch;
            cout << ch;
        }

    }
}