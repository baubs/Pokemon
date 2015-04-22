#include <iostream>
#include "Pokemon.h"
#include "move.h"

int main() {

	Pokemon user1 = Pokemon(25);
	Pokemon user2 = Pokemon(141);

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
				user1.battleDisp();
				cout<<"_______________________"<<endl;
				user2.battleDisp();
				user1.disp_moves();
				cin>>move;

				user1.useMove(move,user2);
				
				cout<<"_______________________"<<endl;
				turn = 2;
				break;
			 case 2:
                                user1.battleDisp();
                                cout<<"_______________________"<<endl;
                                user2.battleDisp();
				user2.disp_moves();
				
                                cin>>move;

                                user2.useMove(move,user1);

				cout<<"_______________________"<<endl;
                                turn = 1;
                                break;


		}
	}	

}
