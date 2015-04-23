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

	user1.LoadMoves(0, 150);
	user1.LoadMoves(1, 147);
	user1.LoadMoves(2, 122);
        user1.LoadMoves(3, 115);

	user2.LoadMoves(0, 119);
        user2.LoadMoves(1, 23);
        user2.LoadMoves(2, 65);
        user2.LoadMoves(3, 41);

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
				system("usleep(70000)");
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
				system("usleep(70000)");
                                break;


		}
	}	

}
