//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//base class for moves
//4/7/2015

#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>
#include "Pokemon.h"

using namespace std;



class move {


	public:
		move();	//defaul constructor
		move(int, string, string, int, int, int, status, int, int);
		void load_move(int m);
		int get_pow();	//get the power of te move
		int get_acc();	//get the accuracy of the move
		int get_pp();	//get the pp of the move
		void display();	//display function
		int reduce_pp();	//reduce the pp of the move
		int reset_pp();	//reset the value of the pp

	

	private:
		int pwr;	//power of the attack
		int acc;	//accuracy of the attack
		int pp;		//power points of hte attack
		int curr_pp;	//current pp
		


};

#endif

