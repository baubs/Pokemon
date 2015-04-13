//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//cpp file for move
//4/10/2015

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Pokemon.h"
#include "move.h"

using namespace std;

//default constructor
move::move(int numb) {

	string trash, typeName;
        ifstream infile;
        infile.open("moveList.txt");

        if (infile.is_open()) {

                getline(infile,trash);

                for (int i = 0; i < numb; i++) {
                        getline(infile, trash);
                }

                //using string stream
                stringstream ss;
                ss << trash;
                ss >> number >> mve >> type >> pwr >> acc >> pp >> stat >> prob;
	}

	curr_pp = pp;

}

//non-default constructor
move::move(int num, string mv, string tpe, int power, int accur, int pPoint, status st, int Probab, int critical) {
	number = num;	//setting number equal to the num (getting the number of the move)
	mve = mv;	//setting move = mve (getting the name of hte move)
	type = tpe;	//setting type = tpe (getting th
	pwr = power;	//setting pwr equal to power
	acc = accur;	//setting acc equal to the accuracy
	pp = pPoint;	//setting pp equal to pPoint
	prob = Probab;	//setting prob equal to probab
	crit = critical; 
	curr_pp = pp;
	stat = st;
}

void move::load_move(int m) {

}

//function to return the power of the move
int move::get_pow() {

	return pwr;
}

//function to return the accuracy of the move
int move::get_acc() {

	return acc;
}

//function to return the power points of the move
int move::get_pp() {

	return pp;
}

//returning the status change of the move
status move::get_status() {

	return stat;
}

//returns the probability of the status change
int move::get_prob() {

	return prob;
}

//returning the chance of a critical hit
int move::get_crit() {

	return crit;
}

void move::display() {

	cout<<number<<mve;

}

//reduces the current pp after te move is used
int move::reduce_pp() {

	curr_pp = curr_pp - 1;	//subtracts 1 from the current pp after a move is used
	return curr_pp;

}

//resets the current pp to the value of the pp
int move:: reset_pp() {
	curr_pp = pp;
}
