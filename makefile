all:	moveTest

moveTest: moveTest.o Pokemon.o move.o
	g++ moveTest.o Pokemon.o move.o -o moveTest

moveTest.o: moveTest.cpp
	g++ moveTest.cpp
Pokemon.o: Pokemon.cpp
	g++ -c Pokemon.cpp
move.o: move.cpp
	g++ -c move.cpp
clear:
	rm -f *.o moveTest
