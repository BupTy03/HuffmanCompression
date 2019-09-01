CC=g++
COMPILER_OPTIONS=-c -std=c++17

all: program

program: main.o HTree.o
	$(CC) main.o HTree.o -o program

main.o: main.cpp 
	$(CC) $(COMPILER_OPTIONS) main.cpp

HTree.o: HTree.cpp HTree.hpp
	$(CC) $(COMPILER_OPTIONS) HTree.cpp HTree.hpp

clean:
	rm -f *.o *.gch program
