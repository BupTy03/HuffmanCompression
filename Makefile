all: program

program: main.o HTree.o
	g++ main.o HTree.o -o program

main.o: main.cpp 
	g++ -c main.cpp

HTree.o: HTree.cpp HTree.hpp
	g++ -c HTree.cpp HTree.hpp

clean:
	rm -f *.o *.gch program
