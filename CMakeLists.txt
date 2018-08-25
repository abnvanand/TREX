cmake_minimum_required(VERSION 3.12)
project(TREX)

set(CMAKE_CXX_STANDARD 14)

add_executable(TREX main.cpp
        list.cpp list.h
        util.cpp util.h
        cursor_control.cpp cursor_control.h
        normal_mode.cpp normal_mode.h
        keymap.h
        command_mode.cpp command_mode.h)
