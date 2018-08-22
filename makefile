CC = g++
CFLAGS = -Wall -std=c++1y
DEPS = util.h list.h
OBJ = util.o list.o main.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

trex: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^



