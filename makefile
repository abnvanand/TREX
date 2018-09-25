# all: library.cpp main.cpp
#
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

CC = g++
CFLAGS = -Wall -std=c++1y
DEPS = util.h list.h cursor_control.h normal_mode.h keymap.h command_mode.h commands.h search_mode.h snapshot.h
OBJ = util.o list.o normal_mode.o cursor_control.o command_mode.o commands.o search_mode.o snapshot.o main.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

trex: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o trex	

