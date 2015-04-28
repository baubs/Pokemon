#include <iostream>
#include <stdlib.h>
#include "Pokemon.h"
#include "moveh"

int main() {

	srand(time(NULL));

	Pokemon wild(rand()%150);

	wild.LoadMoves(0,1+rand()%163);
	wild.LoadMoves(1,1+rand()%163);
	wild.LoadMoves(2,1+rand()%163);
	wild.LoadMoves(3,1+rand()%163);

	
}
