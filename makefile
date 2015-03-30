all:	main

main: main.o Pokemon.o
	g++ main.o Pokemon.o -o main

main.o: main.cpp
	g++ -c main.cpp
Pokemon.o:  Pokemon.cpp
	g++ -c Pokemon.cpp
clean:
	rm -f *.o main
