# TREX
A TeRminal based file EXplorer.


# Assumptions
- Ambiguity in :goto commands path
    - It's mentioned in the assignment pdf file that <directory_path> is relative
    to application root, but it was mentioned in moodle thread that it could also be relative.
    - So I have assumed that a path with no leading '/' or '~' is a path
      relative to current directory.
      i.e., ~/A/B or /A/B looks for the path relative to APP_ROOT
      and A/B looks for the path relative to current directory.
- How to quit the application:
    - Ctrl+C has been disabled in the application so that terminal settings are always restored 
      before the application quits.
    - This means the only way quit the application can be quit is by switching to normal mode
        and press q.


# Known Issues
1. Pressing ESC key application should go back to normal mode. (conflicts with the handling of Arrow Keys)
   An arrow key is a combination of 'ESC','[' and 'A(for up arrow)')
   - workaround: 
        - disable escape in normal mode
             - allows scrolling using arrows
        - enable escape in command mode
             - allows switching to normal mode
             - but can't use arrow keys in command mode.
2. 