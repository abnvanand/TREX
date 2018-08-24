#include "normal_mode.h"
#include "keymap.h"

using namespace std;

// We are in Normal mode (where you can't type anything exceptions are always there :)
void switch_to_normal_mode() {

    struct termios initial_settings, new_settings;
    char ch;

    tcgetattr(fileno(stdin), &initial_settings);

    new_settings = initial_settings;

    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;         // to hide typed entry
    new_settings.c_lflag &= ~ISIG;          // to catch ctrl^c etc signal

    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;


    if (tcsetattr(fileno(stdin), TCSANOW, &new_settings) != 0) {
        cerr << "Could not set attributes\n";
    } else {
        do {
            cin >> ch;

            switch (ch) {
                case ARROW_UP:
                    move_up(1);
                    break;
                case ARROW_DOWN:
                    move_down(1);
                    break;
                case ARROW_RIGHT:
                    move_right(1);
                    break;
                case ARROW_LEFT:
                    move_left(1);
                    break;


                case ETX:  // ETX (end of text) (Ctrl+C )
                    cout << "\nType q to quit\n";
                    break;
                case COLON:
                    move_cursor_xy(10, 10);
                    // TODO: move to command mode
                    cout << "\nCommand mode\n";
                    break;
                case QUIT:
                    break;

                    /* FIXME: arrow keys give multiple characters 'ESC','[' and The actual arrowkey(A,B,C,D)
                     So suppress for now until we implement COMMAND Mode
                     */
                case ESCAPE:// ESCape key
                    //cout << "\nNormal mode\n";
                    break;
                default:
                    //cout << "\n You entered: " << ch << '\n';
                    break;
            }

        } while (ch != 'q');

        // MUST restore the terminal to the way it was
        tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
    }
}