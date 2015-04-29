all:	moveTest

moveTest: moveTest.o Pokemon.o move.o
	g++ moveTest.o Pokemon.o move.o -lSDL_image -lSDL_ttf -o moveTest

moveTest.o: moveTest.cpp
	g++ -c moveTest.cpp
Pokemon.o: Pokemon.cpp
	g++ -c Pokemon.cpp
move.o: move.cpp
	g++ -c move.cpp
clean:
	rm -f *.o moveTest
