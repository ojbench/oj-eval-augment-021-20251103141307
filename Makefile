CXX := g++-13
CXXFLAGS := -O2 -std=gnu++17 -pipe -s -Wall

all: code

code: main.cpp game.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp

clean:
	rm -f code *.o

