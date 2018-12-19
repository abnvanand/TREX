CXX = g++
# -Wall = flag for all warnings
# -g = adds debugging information (so that the executable can later be run under a debugger).
CXXFLAGS = -Wall -g -std=c++1z

FILES   := $(wildcard src/*.cpp)
OBJECTS := $(FILES:src/%.cpp=obj/%.o)

# example:-
# all: library.cpp main.cpp
#
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

./obj/%.o: ./src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

trex: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o trex	obj/*.o

