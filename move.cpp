//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//cpp file for move
//4/10/2015

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//#include "Pokemon.h"
#include "move.h"

using namespace std;



//non-default constructor
move::move(int numb) {

	string trash, typeName, statusName, statsName;
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
                ss >> number >> mve >> typeName >> pwr >> acc >> pp >> statusName >> prob >> specialTag;
		ss >> selfChange >> foeChange >> statsName;
                typeFromText(typeName);
		typeFromText_2(statusName);
		statsFromText(statsName);
	}

	curr_pp = pp;

}

//non-default constructor
move::move(int num, string mv, types tpe, int power, int accur, int pPoint, status st, float Probab) {
	number = num;	//setting number equal to the num (getting the number of the move)
	mve = mv;	//setting move = mve (getting the name of hte move)
	type = tpe;	//setting type = tpe (getting th
	pwr = power;	//setting pwr equal to power
	acc = accur;	//setting acc equal to the accuracy
	pp = pPoint;	//setting pp equal to pPoint
	prob = Probab;	//setting prob equal to probab
	curr_pp = pp;
	stat = st;
}

move::move() {
	number = 1;

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
float move::get_prob() {

	return prob;
}

//returning the chance of a critical hit
float move::get_crit() {

	return crit;
}

void move::display() {

	cout<<number<<" "<<mve<<" "<<getTypeText(type)<<" "<<pwr<<" "<<acc<<" "<<pp<<" "<<getStatusText()<<" "<<specialTag<<endl;

}

//reduces the current pp after te move is used
int move::reduce_pp() {

	curr_pp = curr_pp - 1;	//subtracts 1 from the current pp after a move is used
	return curr_pp;

}

string move::getName()
{
	return mve;
}

//resets the current pp to the value of the pp
int move:: reset_pp() {
	curr_pp = pp;
}


//sets type value given a text, used in loading function
void move::typeFromText(string t)
{
	if(t == "Normal")
		type = Normal;
	else if(t == "Grass")
		type = Grass;
	else if(t == "Water")
		type = Water;
	else if(t == "Fire")
		type = Fire;
	else if(t == "Flying")
		type = Flying;
	else if(t == "Fight")
		type = Fight;
	else if(t == "Psychic")
		type = Psychic;
	else if(t == "Bug")
		type = Bug;
	else if(t == "Poison")
		type = Poison;
	else if(t == "Eletric")
		type = Eletric;
	else if(t == "Rock")
		type = Rock;
	else if(t == "Ground")
		type = Ground;
	else if(t == "Ghost")
		type = Ghost;
	else if(t == "Dark")
		type = Dark;
	else if(t == "Ice")
		type = Ice;
	else if(t == "Dragon")
		type = Dragon;
	else
      type = Normal;

}

//returns text version of status
string move::getStatusText()
{
  string toRet;
  switch(stat)
  {
    case normal:
      toRet = "Normal";
      break;
    case burned:
      toRet = "Burned";
      break;
    case poisoned:
      toRet = "Poisoned";
      break;
    case asleep:
      toRet = "Sleep";
      break;
    case paralyzed:
      toRet = "Paralyzed";
      break;
    case frozen:
      toRet = "Frozen";
      break;
    case fainted:
      toRet = "Faint";
      break;
    default:
      toRet = "ERROR";
      break;
  }
  return toRet;
}

string move::getStatText()
{
	string toRet;

	switch(change)
	{
		case atk:
			toRet = "atk";
			break;
		case def:
			toRet = "def";
			break;
		case spAtk:
			toRet = "spAtk";
			break;
		case spDef:
			toRet = "spDef";
			break;
		case speed:
			toRet = "speed";
			break;
		case HP:
			toRet = "HP";
			break;
		case maxHP:
			toRet = "maxHP";
			break;
		default:
			toRet = "ERROR";
			break;
	}

	return toRet;
}

//returns type in text
string move::getTypeText(types t)
{
  string toRet;
  switch(t)
  {
    case Normal:
      toRet = "Normal";
      break;
    case Grass:
      toRet = "Grass";
      break;
    case Water:
      toRet = "Water";
      break;
    case Fire:
      toRet = "Fire";
      break;
    case Flying:
      toRet = "Flying";
      break;
    case Fight:
      toRet = "Fight";
      break;
    case Psychic:
      toRet = "Psychic";
      break;
    case Bug:
      toRet = "Bug";
      break;
    case Poison:
      toRet = "Poison";
      break;
    case Eletric:
      toRet = "Eletric";
      break;
    case Rock:
      toRet = "Rock";
      break;
    case Ground:
      toRet = "Ground";
      break;
    case Ghost:
      toRet = "Ghost";
      break;
    case Dark:
      toRet = "Dark";
      break;
    case Ice:
      toRet = "Ice";
      break;
    case Dragon:
      toRet = "Dragon";
      break;
  }
  return toRet;
}

void move::statsFromText(string s)
{

	if(s == "atk") change = atk;

	else if(s == "def") change = def;
	
	else if(s == "spAtk") change = spAtk;

	else if(s == "spDef") change = spDef;

	else if(s == "speed") change = speed;

	else if(s == "HP") change = HP;

	else change = maxHP;
	
}

void move::typeFromText_2(string t) {
	
	if (t == "normal") stat = normal;
	
	else if (t == "burned") stat = burned;

	else if (t == "poisoned") stat = poisoned;

	else if (t == "asleep") stat = asleep;

	else if (t == "paralyzed") stat = paralyzed;

	else if (t == "frozen") stat = frozen;

	else if (t == "fainted") stat = fainted;

	else stat = normal;
}

void move::BattleDisplay() {

	cout<<mve<<endl;
	cout<<getTypeText(type)<<" PP "<<curr_pp<<"/"<<pp<<endl;
}

int move::getSpecial() {

	return specialTag;
}

types move::get_type() {

	return type;
}

int move::getSelfChange()
{
	return selfChange;
}

int move::getFoeChange()
{
	return foeChange;
}

stats move::getChangeStat()
{
	return change;
}
