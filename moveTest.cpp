#include <iostream>
#include <stdlib.h>
#include "Pokemon.h"
#include "move.h"

int main() {

	int A, B;
	int moveA[4];
	int moveB[4];

	cout << "Pick a pokemon player 1. " << endl;
	cin >> A;
	cout << "Pick a pokemon player 2. " << endl;
        cin >> B;



	Pokemon user1 = Pokemon(A);
	Pokemon user2 = Pokemon(B);

	user1.LoadMoves(0, 39);
	user1.LoadMoves(1, 99);
	user1.LoadMoves(2, 141);
        user1.LoadMoves(3, 23);

	user2.LoadMoves(0, 77);
        user2.LoadMoves(1, 99);
        user2.LoadMoves(2, 105);
        user2.LoadMoves(3, 41);
	

	user1.disp();

	int turn = 2;
	int move = 0;

	if (user1.getSpeed() > user2.getSpeed())
		turn = 1;

	while (user1.getStatus() != fainted && user2.getStatus() != fainted) {


		

		switch(turn)
		{
			case 1:
				system("clear");
				user1.battleDisp();
				cout<<"_______________________"<<endl;
				user2.battleDisp();
				user1.disp_moves();
				cin>>move;
				
				user1.useMove(move,user2);
				
				cout<<"_______________________"<<endl;
				turn = 2;
				sleep(5);
				user1.gainexp(user1.giveXP());
				break;
			 case 2:
				system("clear");
                                user1.battleDisp();
                                cout<<"_______________________"<<endl;
                                user2.battleDisp();
				user2.disp_moves();
				
                                cin>>move;

                                user2.useMove(move,user1);

				cout<<"_______________________"<<endl;
                                turn = 1;
                                sleep(5);
				break;


		}
	}
	user1.savePokemon();	

}
