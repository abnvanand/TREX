#include <strings.h>
#include <cstring>
#include "command_mode.h"
#include "keymap.h"
#include "util.h"
#include "cursor_control.h"

using namespace std;

int command_mode() {
    int cause_of_return = RES_QUIT;

    draw_info_line("COMMAND MODE");
    draw_command_line();


    /*
     * Initialize termios
    */
    struct termios initial_settings;
    tcgetattr(fileno(stdin), &initial_settings);

    struct termios new_settings = initial_settings;

    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;         // to hide typed entry
    new_settings.c_lflag &= ~ISIG;         // to catch ctrl^c etc signal

    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;


    if (tcsetattr(fileno(stdin), TCSAFLUSH, &new_settings) != 0) {
        cerr << "Could not set attributes\n";
        return RES_ERROR;
    }


    string command;

    do {
        char ch;
        // can't do cin>>ch https://stackoverflow.com/questions/42818899/detecting-enter-key-in-c
        ch = (char) cin.get();

        if ((ch == BACKSPACE || ch == DELETE) && command.length() > 0) {
            // FIXME Also deletes the colon ":"
            cout << "\b \b";
            if (command.length() == 1)
                command = "";
            else
                command = command.substr(0, command.length() - 1);
        } else if (ch == ESCAPE) {
            cause_of_return = RES_NORMAL_MODE;
            break;
        } else if (ch == ENTER) {
            execute_command(command);
            command = "";   // reset command
        } else {
            command += ch;
            cout << ch;
        }

    } while (true);

    // MUST restore the terminal to the way it was
    tcsetattr(fileno(stdin), TCSANOW, &initial_settings);

    return cause_of_return;
}

void execute_command(string &command) {
    draw_command_line();
    cout << "Executing command :" << command;
    // TODO: break into tokens to be used to detect the type of command
}
