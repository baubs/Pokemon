//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//Base class for all Pokemon
//03/25/2015

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include "Pokemon.h"
#include "move.h"
#include "enums.h"
using namespace std;

Pokemon:: Pokemon()
{
  srand(time(NULL));
  name = "Pikachu";
  nickname = "Pika";
  type = Eletric;
  number = 19;
  level = 1;
  exp = 0;
  nxtLev = 50;
  stats[HP] = 20;
  stats[maxHP] = stats[HP];
  stats[atk] = 1;
  stats[def] = 1;
  stats[spAtk] = 1;
  stats[spDef] = 1;
  stats[speed] = 1;
  mults[HP] = 0;      //do not want to add on to current HP
  mults[maxHP] = 2;
  mults[atk] = 1;
  mults[def] = 1;
  mults[spAtk] = 1;
  mults[spDef] = 1;
  effect = normal;
}


//non default constructor 
Pokemon:: Pokemon(int num, string n, string nn, int lev, int nxtLevel,int stat[7], int muls[7],status eff, types t)
{
  srand(time(NULL)); 
   
  number = num;
  name = n;
  nickname = nn;
  level = lev;
  exp = 0;
  nxtLev = nxtLevel;
  type = t;
  
  for(int i = 0; i < 7; i ++)
  {
    stats[i] = stat[i];
    mults[i] = muls[i];
  }

  effect = eff;
}

//destructor
Pokemon::~Pokemon()
{
  delete []moveSet;
}

//Constructor only takes int and loads from file Pokelist
Pokemon::Pokemon(int num)
{
  srand(time(NULL));
  if(num < 1 || num > 150)
  {
     cout << "Number should be between 1-150" << endl;
     return;
  }
  
  string trash, typeName;
  ifstream infile;
  infile.open("PokeList.txt");

  if (infile.is_open())
  {
    getline(infile, trash);       //gets the header line out of the way
    for(int i = 0; i < num; i++)  //gets all the way up to the poke of intrest
    {
      getline(infile, trash); 
    }

    //using string stream 
    stringstream ss;
    ss << trash;
    ss >> number >> name >> typeName >> stats[HP] >> stats[atk] >> stats[def];
    ss >> stats[spAtk] >> stats[spDef] >> stats[speed];
    ss >> mults[HP] >> mults[atk] >> mults[def] >> mults[spAtk] >> mults[spDef] >> mults[speed]; 

  }
  nickname = name;
  level = 1;
  exp = 0;
  nxtLev = 100;
  stats[maxHP] = stats[HP];
  effect = normal;
  typeFromText(typeName);
  moveSet = new move[4];
}

void Pokemon::LoadMoves(int index, int moveNumber) {
  moveSet[index] = move(moveNumber);
}


//algorithm  for using move on a pokemon
void Pokemon::useMove(int index, Pokemon &other)
{
	int attackPow;     //atk stat
	int defensePow;    //def stat
	double typeBonus;  //1.5 or 1
	int random = (rand()%38)+217;//random num b/w 217-255
	int hit = (rand()%100);      //random number to check if move hit or not
	status s = moveSet[index].get_status();   //gets status effect of move

	//checks typing effectivness against target type
	double typeEffect = other.checkTyping(moveSet[index].get_type()); 

	//checking for paralazyed movment 
	int parCheck = rand()%2;
	if(getStatus() == paralyzed && parCheck == 1)
		cout << getNickName() << " is paralyzed! It can't move!" << endl;

	//handleing frozen and asleep pokemon
	else if(getStatus() == asleep || getStatus() == frozen && parCheck != 1)
		moveHelpAsleepFrozen();

	//if the move hit
	else if(hit < moveSet[index].get_acc() && parCheck != 1)
	{
		//if move type is same as pokemon type the move gains .5 power
		if(getType() == moveSet[index].get_type())
		{
			typeBonus = 1.5;
		}
		else
		{
			typeBonus = 1;
		}
		//gets proper atk and def stats based on atk tag (Physical/special)
		//if tag is 1 it is special, 0 is physical
		if(moveSet[index].getSpecial())
		{
			attackPow = getSpAtk();
			defensePow = other.getSpDef();
		}
		else
		{
			attackPow = getAtk();
			defensePow = other.getSpDef();
		}

		//algorithm broken into 3 parts atk based, defense based, random based
		double atkPart = ((2*getLevel()/5+2)*attackPow*moveSet[index].get_pow()); 
		double defPart = ((((atkPart/defensePow)/50)+2)*typeBonus)*(typeEffect/10);
		double randPart = (defPart*random)/255;

		//final damage done
		int rounded = (int)(round(randPart));
		if(moveSet[index].get_pow() == 0)
			rounded = 0;

		//interactive text
		cout << getNickName() << " used " << moveSet[index].getName();
		if(typeEffect == 10 || rounded == 0)
			cout << "." << endl;
		else if(typeEffect == 20)
			cout << ", its super effective." << endl;
		else if(typeEffect == 5)
			cout << ", its not very effective." << endl;
		else if(typeEffect == 0)
			cout << ", it has no effect on foe " << other.getNickName() << "." << endl;
		else
			cout << ", and it is a broken move." << endl;
		
		//passes on status effect to other pokemon
		int statusProc = rand()%100;
		if(statusProc < moveSet[index].get_prob() && moveSet[index].get_status() != normal)
		{
			if(other.getStatus() == normal)
			{
				status change = moveSet[index].get_status();
				other.changeStatus(change);
				cout << other.getNickName() << " is " << other.getStatusText() << endl;
			}
			else
				cout << "it had no effect on foe's status." << endl;
		}
		//update other pokemon's hp and moves pp
		other.reduceHP(rounded);
		moveSet[index].reduce_pp();
	}
	//move missed
	else
	{	
		cout << getNickName() << " used " << moveSet[index].getName() << ", it missed." << endl;
		moveSet[index].reduce_pp();
	}

	//burn and poison damages
	if(getStatus() == burned)
		moveHelpBurn();
	else if(getStatus() == poisoned)
		moveHelpPoison();	
}

//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
void Pokemon::moveHelpBurn()
{
	double dmg = getMaxHP()/10;
	reduceHP((int)(dmg));
	cout << getNickName() << " was hurt by the burn." << endl;
}
//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
void Pokemon::moveHelpPoison()
{
        double dmg = getMaxHP()/10;
        reduceHP((int)(dmg));
	cout << getNickName() << " was hurt by the poison." << endl;
}
//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
void Pokemon::moveHelpAsleepFrozen()
{
	int check = rand()%2;

	if(check == 1 && getStatus() == asleep)
	{
		changeStatus(normal);
		cout << getNickName() << " has woken up." << endl;
	}
	else if(check)
        {
                changeStatus(normal);
                cout << getNickName() << " has thawed." << endl;
        }
	else if(getStatus() == asleep)
		cout << getNickName() << " is fast asleep." << endl;
	else
		cout << getNickName() << " is frozen solid." << endl;
}


//helps Pokemon constructor that loads from a file
void Pokemon::typeFromText(string t)
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

//level up the pokemon
void Pokemon:: levelup()
{
  level ++;
  exp = 0;
  nxtLev *= 1.2;  

  //add onto stats using multipliers
  stats[maxHP] += mults[maxHP];
  stats[atk] += mults[atk];
  stats[def] += mults[def];
  stats[spAtk] += mults[spAtk];
  stats[spDef] += mults[spDef];
  stats[speed] += mults[speed];
}

//giving a pokmon exp
void Pokemon:: gainexp(int xp)
{
  //if pokemon gains more exp than it needs to 
  if(exp + xp >= nxtLev)
  {
    int nextXP = exp + xp - nxtLev;
    levelup();                       
    gainexp(nextXP);   //spill over exp
  }
  else
    exp += xp;
}

//change status of pokemon
void Pokemon:: changeStatus(status eff)
{
  effect = eff;
}

//reduce HP of pokemon
void Pokemon:: reduceHP(int lost)
{
  //if pokemon's HP is now 0
  if(stats[HP] - lost <= 0)
  {
    stats[HP] = 0;
    effect = fainted;
  }
  else
    stats[HP] -= lost;
}

//getters nothing too interesting
status Pokemon:: getStatus()
{
  return effect;
}

int Pokemon:: getLevel()
{
  return level;
}

int Pokemon:: getExp()
{
  return exp;
}

int Pokemon:: getMaxExp()
{
  return nxtLev;
}

string Pokemon:: getName()
{
  return name;
}

string Pokemon:: getNickName()
{
  return nickname;
}

int Pokemon:: getHP()
{
  return stats[HP];
}

int Pokemon:: getMaxHP()
{
  return stats[maxHP];
}

int Pokemon:: getAtk()
{
  return stats[atk];
}

int Pokemon:: getDef()
{
  return stats[def];
}

int Pokemon:: getSpAtk()
{
  return stats[spAtk];
}

int Pokemon:: getSpDef()
{
  return stats[spDef];
}

int Pokemon:: getSpeed()
{
  return stats[speed];
}

int Pokemon::getNumber()
{
  return number;
}

types Pokemon::getType()
{
  return type;
}

//returns text version of status
string Pokemon::getStatusText()
{
  string toRet;
  switch(effect)
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

//returns type in text
string Pokemon::getTypeText(types t)
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

//Display function
void Pokemon::disp()
{
  cout << "#" << getNumber() << " " << getName() << ", \"" << getNickName() << "\"" << endl;
  cout << "Type: " << getTypeText(getType()) << ", Level: " << getLevel() << endl;
  cout << "Status: " << getStatusText() << endl;
  cout << "EXP: " << getExp() << "/" << getMaxExp() << endl;
  cout << "HP: " << getHP() << "/" << getMaxHP() << "     Speed: " << getSpeed() << endl;
  cout << "Atk: " << getAtk() << "\t" << "Def: " << getDef() << endl;
  cout << "SpA: " << getSpAtk() << "\t" << "SpD: " << getSpDef() << endl;
}

//limited display for battle sceens
void Pokemon::battleDisp()
{
  cout << getNickName() << " : lvl " << getLevel() << endl;
  cout <<  "HP: " << getHP() << "/" << getMaxHP();
  if(getStatus() != normal)
    cout << " " << getStatusText() << endl;
  else
    cout << endl;
}

//checks typing, 2 = no effect; 1 = super; 0 = normal; -1 = resistant 
int Pokemon::checkTyping(types t)
{
  switch(type)
  {
    case Normal:
      if(t == Fight)
        return 20;
      else if(t == Ghost)
        return 0;
      else
        return 10;
      break;

    case Grass:
      if(t == Fire || t == Ice || t == Poison || t == Flying)
        return 20;
      else if(t == Water || t == Eletric || t == Grass || t == Ground)
        return 5;
      else
        return 10;
      break;

    case Water:
      if(t == Eletric || t == Grass)
        return 20;
      else if(t == Fire || t == Water || t == Ice)
        return 5;
      else
        return 10;
      break;

    case Fire:
      if(t == Water || t == Ground || t == Rock)
        return 20;
      else if(t == Fire || t == Grass || t == Ice || t == Bug)
        return 5;
      else
        return 10;
      break;

    case Flying:
      if(t == Eletric || t == Rock || t == Ice)
        return 20;
      else if(t == Fight || t == Grass ||  t == Bug)
        return 5;
      else if(t == Ghost)
        return 0;
      else
        return 10;
      break;

    case Fight:
      if(t == Flying || t == Psychic)
        return 20;
      else if(t == Rock || t == Dark || t == Bug)
        return 5;
      else
        return 10;
      break;

    case Psychic:
      if(t == Bug || t == Ghost || t == Dark)
        return 20;
      else if(t == Psychic || t == Fight)
        return 5;
      else
        return 10;
      break;

    case Bug:
      if(t == Fire || t == Flying || t == Rock)
        return 20;
      else if(t == Fight || t == Grass || t == Ground)
        return 5;
      else
        return 10;
      break;

    case Poison:
      if(t == Ground || t == Psychic)
        return 20;
      else if(t == Bug || t == Grass || t == Fight || t == Poison)
        return 5;
      else
        return 10;
      break;

    case Eletric:
      if(t == Ground)
        return 20;
      else if(t == Eletric || t == Flying)
        return 5;
      else
        return 10;
      break;

    case Rock:
      if(t == Water || t == Ground || t == Fight || t ==  Grass)
        return 20;
      else if(t == Normal || t == Fire || t == Poison || t == Flying)
        return 5;
      else
        return 10;
      break;

    case Ground:
      if(t == Water || t == Grass || t == Ice)
        return 20;
      else if(t == Rock || t == Poison)
        return 5;
      else if(t == Eletric)
        return 0;
      else
        return 10;
      break;

    case Ghost:
      if(t == Ghost || t == Dark)
        return 20;
      else if(t == Poison || t == Bug)
        return 5;
      else if(t == Normal || t == Fight)
        return 0;
      else
        return 10;
      break;

    case Dark:
      if(t == Fight || t == Bug)
        return 20;
      else if(t == Ghost || t == Dark)
        return 5;
      else if(t == Psychic)
        return 0;
      else
        return 10;
      break;

    case Ice:
      if(t == Fire || t == Fight || t == Rock)
        return 20;
      else if(t == Ice)
        return 5;
      else
        return 10;
      break;

    case Dragon:
      if(t == Ice || t == Dragon)
        return 20;
      else if(t == Fire || t == Grass || t == Water || t == Eletric)
        return 5;
      else
        return 10;
      break;
  }
}

//displays one move
void Pokemon::disp_move(int i) {

	cout << i << ": ";
	moveSet[i].BattleDisplay();
}

//displays the whole moveset
void Pokemon::disp_moves()
{
	cout << "-------------------" << endl;
	for(int i = 0; i < 4; i++)
	{
		disp_move(i);
		cout << "-------------------" << endl;
	}
}
