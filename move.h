//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//base class for moves
//4/7/2015

#ifndef MOVE_H
#define MOVE_H


#include <iostream>
#include <string>
#include "enums.h"

using namespace std;


class move {


	public:
		move();
		move(int);	//defaul constructor
		move(int, string, types, int, int, int, status, float);
		void load_move(int m);
		int get_pow();	//get the power of te move
		int get_acc();	//get the accuracy of the move
		int get_pp();	//get the pp of the move
		status get_status();	//get the status
		types get_type();
		float get_prob();	//get the probability of the status change
		float get_crit();	//probability of critical hit
		void display();	//display function
		int reduce_pp();	//reduce the pp of the move
		int reset_pp();	//reset the value of the pp
		void typeFromText(string);
		void typeFromText_2(string);
		void statsFromText(string);
		string getName();
		string getStatusText();
		string getTypeText(types);
		string getStatText();	
		void BattleDisplay();
		int getSpecial();
		int getSelfChange();
		int getFoeChange();
		stats getChangeStat();		
	private:
		int pwr;	//power of the attack
		int acc;	//accuracy of the attack
		int pp;		//power points of hte attack
		int curr_pp;	//current pp
		int number;	//the number associated with the move on the list
		types type;	//the type of the move
		status stat;	//the status change that the move causes
		float prob;	//probability of the status change
		float crit;	//probability of a critical hit
		string mve;	//the name of the move;
		int specialTag; //if move is a special this is 1 if not it is 0	
		int selfChange;	//amount the move modifies own stats
		int foeChange;  //amount the move effects foe's stats
		stats change;	//stat modified

};

#endif

