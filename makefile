all:	basicTest

basicTest: basicTest.o Pokemon.o
	g++ basicTest.o Pokemon.o -o basicTest

basicTest.o: basicTest.cpp
	g++ -c basicTest.cpp
Pokemon.o:  Pokemon.cpp
	g++ -c Pokemon.cpp
clean:
	rm -f *.o basicTest
