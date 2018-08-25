#include "normal_mode.h"
#include "util.h"
#include "list.h"

using namespace std;

// We are in Normal mode (where you can't type anything exceptions are always there :)
/**
 * receives initial termios settings and modifies its copy
 * it's the responsibility of this module to leave the terminal in the state it was
 * @param initial_settings
 * @return
 */
int normal_mode() {
//    clear_screen();

    draw_info_line("NORMAL_MODE");
    move_cursor_xy(1, 1);

    cout << "-------------BEGIN-----------" << endl;
    ls(".");
    cout << "--------------END-----------" << endl;

    move_cursor_xy(1, 1);

    struct termios initial_settings, new_settings;
    tcgetattr(fileno(stdin), &initial_settings);
    new_settings = initial_settings;

    char ch;
    int cause_of_return = RES_QUIT;

    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;         // to hide typed entry
    new_settings.c_lflag &= ~ISIG;         // to catch ctrl^c etc signal

    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;


    if (tcsetattr(fileno(stdin), TCSANOW, &new_settings) != 0) {
        cerr << "Could not set attributes\n";
        return RES_ERROR;
    }

    do {
        cin >> ch;

        switch (ch) {
            case ARROW_UP:
            case ARROW_DOWN:
//            case ARROW_RIGHT:
//            case ARROW_LEFT:
                move_cursor(ch);
                break;

            case ETX:  // ETX (end of text) (Ctrl+C )
                cout << "\nType q to quit\n";
                break;
            case QUIT:
                // DO NOT return here. Need to restore the termios initial settings
                cause_of_return = RES_QUIT;
                break;

            case COLON:
                // FIXME: CAN NOT and MUST NOT Return here before restoring terminal state
                cause_of_return = RES_COMMAND_MODE;
                break;

            default:
                // cout << "\n You entered: " << ch << '\n';
                break;
        }

    } while (ch != 'q' && ch != ':');

    // MUST restore the terminal to the way it was
    tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
    return cause_of_return;
}