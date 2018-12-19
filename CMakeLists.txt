cmake_minimum_required(VERSION 3.12)
project(TREX)

set(CMAKE_CXX_STANDARD 14)

add_executable(TREX src/main.cpp
        src/list.cpp src/list.h
        src/util.cpp src/util.h
        src/cursor_control.cpp src/cursor_control.h
        src/normal_mode.cpp src/normal_mode.h
        src/keymap.h
        src/command_mode.cpp src/command_mode.h
        src/commands.cpp src/commands.h
        src/search_mode.cpp src/search_mode.h
        src/snapshot.cpp src/snapshot.h)
