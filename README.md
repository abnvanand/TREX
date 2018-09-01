# TREX
A TeRminal based file EXplorer.

# Usage
Clean previous build if any
```commandline
make clean
```
Compile
```commandline
make
```
Run application
```
./trex
```
Exiting application
```
Return to normal mode by pressing ESC if currently in command mode.
Then press q to quit.
```

# Assumptions
1. rename: assuming file path is relative to current directory

2. Ambiguity in `:goto` commands path
    - It's mentioned in the assignment pdf file that <directory_path> is relative
    to application root, but it was mentioned in moodle thread that it could also be relative.
    - So I have assumed that a path with no leading '/' or '~/' is a path
      relative to current directory.
      i.e., `~/A/B or /A/B` looks for the path `relative to APP_ROOT`
      whereas `A/B` looks for the path relative to `current directory`.
      
3. Similarly following path assumption have been made for the corresponding commands:- 
    - copy : directory name can be absolute or relative.
    - move : directory name can be absolute or relative.
    - rename : file with <filename> is present in current directory. No path should be specified
    - create_file : directory name can be absolute or relative.
    - create_folder : destination directory name can be absolute or relative.
    - delete_file <file_path> : <file_path> is always relative to APP ROOT(as per assignment pdf) 
      so no ~ or / as prefix is allowed
    - delete_dir <directory_path> : <directory_path> is always relative to APP ROOT(as per assignment pdf)
      so no ~ or / as prefix is allowed
    - goto <directory_path> : <directory_path> is relative to APP_ROOT

4. Directory copy: copy a.txt ~/B C
    will copy the file a.txt to directory C and 
    directory B and all its contents into directory C
    i.e, now C will contain a directory named B.
    - Both relative and absolute paths work.

5. Space in file names:
    - User can enter space by quoting them with single or double quotes. 
      The only requirement is that the starting and ending quotes must match.
     eg: `create_file "Abhinav's CV.txt" . `
     creates a new file named `Abhinav's CV.txt` in current directory
    
6. New file is created with permissions 0644
  i.e., READ,WRITE for owner, READ for group, READ for others. 
  
7. How to quit the application:
    - Ctrl+C has been disabled in the application so that terminal settings are always restored 
      before the application quits.
    - This means the only way quit the application can be quit is by switching to normal mode
        and press q.

8. Deleting directory currently being shown in normal mode.
    - what should happen?
    - on linux it gets deleted.
    - in TREX as well it will get deleted
    - and when switching to the normal mode the user will be taken to home.

# Known Issues
1. Pressing ESC key application should go back to normal mode. (conflicts with the handling of Arrow Keys)
   An arrow key is a combination of 'ESC','[' and 'A(for up arrow)')
   - workaround: 
        - disable escape in normal mode
             - allows scrolling using arrows
        - enable escape in command mode
             - allows switching to normal mode
             - but can't use arrow keys in command mode.