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
  baseStats[HP] = 20;
  baseStats[maxHP] = baseStats[HP];
  baseStats[atk] = 1;
  baseStats[def] = 1;
  baseStats[spAtk] = 1;
  baseStats[spDef] = 1;
  baseStats[speed] = 1;
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
    baseStats[i] = stat[i];
    tempStats[i] = stat[i];
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
    ss >> number >> name >> typeName >> baseStats[HP] >> baseStats[atk] >> baseStats[def];
    ss >> baseStats[spAtk] >> baseStats[spDef] >> baseStats[speed];
    ss >> mults[HP] >> mults[atk] >> mults[def] >> mults[spAtk] >> mults[spDef] >> mults[speed]; 

  }
  nickname = name;
  level = 1;
  exp = 0;
  nxtLev = 100;

  baseStats[maxHP] = baseStats[HP];
  for(int i = 0; i < 7; i ++)
  {
    tempStats[i] = baseStats[i];
  }

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
	else if(getStatus() == asleep || getStatus() == frozen )
		moveHelpAsleepFrozen();

	//if the move hit
	else if(hit < moveSet[index].get_acc() )
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
		//gets proper atk and def baseStats based on atk tag (Physical/special)
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
		statsChange(index, rounded,  other);
	}
	//move missed
	else
	{	
		cout << getNickName() << " used " << moveSet[index].getName() << ", it missed." << endl;
		moveSet[index].reduce_pp();
	}

	//burn and poison damages
	if(getStatus() == burned)
		moveHelpBurn(other);
	else if(getStatus() == poisoned)
		moveHelpPoison(other);

	//checks if move KOed other pokemon
	if(other.getHP() == 0)
	{
		cout << other.getNickName() << " fainted." << endl;
		other.resetTemp();
		resetTemp();
	}	
}

//deals with status changes in use moves
void Pokemon::statsChange(int index, int damageDone, Pokemon &other)
{
	//for HP stat
	switch( moveSet[index].getChangeStat() )
	{
		int self, foe;

		case HP:
			if( moveSet[index].getSelfChange() == 5 )
			{
				gainHP(damageDone/2);
				cout << getNickName() << " absorbed health." << endl;
			}
			else if( moveSet[index].getSelfChange() < 0 )
			{
				gainHP(moveSet[index].getSelfChange());
				cout << getNickName() << " was hit by recoil." << endl;
				if(getHP() == 0)
					cout << getNickName() << " has fainted." << endl;
			}
			else if( moveSet[index].getSelfChange() > 0)
			{
                        	gainHP(moveSet[index].getSelfChange());
                        	cout << getNickName() << " recovered health." << endl;
			}
			other.gainHP(moveSet[index].getFoeChange());
			break;
		case speed:
			self = moveSet[index].getSelfChange(); 
			if( self > 0 && tempStats[speed] - baseStats[speed] < 60)
			{
				tempStats[speed] += self;
				cout << getNickName() << "'s speed rose." << endl;
			}
			else if(self > 0)
			{
				cout << getNickName() << "'s speed won't go higher." << endl;
			}
			else if( self < 0 && tempStats[speed] + self >= 5)
			{
				tempStats[speed] += self;
				cout << getNickName() << "'s speed fell." << endl;
			}
			else if( self < 0 )
			{
				cout << getNickName() << "'s speed won't go lower." << endl;
			}

			foe = moveSet[index].getFoeChange();
                        if( foe > 0 && other.tempStats[speed] - other.baseStats[speed] < 60)
                        {
                                other.tempStats[speed] += foe;
                                cout << other.getNickName() << "'s speed rose." << endl;
                        }
                        else if(foe > 0)
                        {
                                cout << other.getNickName() << "'s speed won't go higher." << endl;
                        }
                        else if( foe < 0 && other.tempStats[speed] + foe >= 5)
                        {
                                other.tempStats[speed] += foe;
                                cout << other.getNickName() << "'s speed fell." << endl;
                        }
                        else if( foe < 0 )
                        {
                                cout << other.getNickName() << "'s speed won't go lower." << endl;
                        }
			break;

		case atk:
			self = moveSet[index].getSelfChange();
                        if( self > 0 && tempStats[atk] - baseStats[atk] < 60)
                        {
                                tempStats[atk] += self;
                                cout << getNickName() << "'s atk rose." << endl;
                        }
                        else if(self > 0)
                        {
				tempStats[atk] = baseStats[atk] + 60;
                                cout << getNickName() << "'s atk won't go higher." << endl;
                        }
                        else if( self < 0 && tempStats[atk] + self >= 5)
                        {
                                tempStats[atk] += self;
                                cout << getNickName() << "'s atk fell." << endl;
                        }
                        else if( self < 0 )
                        {
                                cout << getNickName() << "'s atk won't go lower." << endl;
                        }

                        foe = moveSet[index].getFoeChange();
                        if( foe > 0 && other.tempStats[atk] - other.baseStats[atk] < 60)
                        {
                                other.tempStats[atk] += foe;
                                cout << other.getNickName() << "'s atk rose." << endl;
                        }
                        else if(foe > 0)
                        {
				other.tempStats[atk] = other.baseStats[atk] + 60;
                                cout << other.getNickName() << "'s atk won't go higher." << endl;
                        }
                        else if( foe < 0 && other.tempStats[atk] + foe >= 5)
                        {
                                other.tempStats[atk] += foe;
                                cout << other.getNickName() << "'s atk fell." << endl;
                        }
                        else if( foe < 0 )
                        {
                                cout << other.getNickName() << "'s atk won't go lower." << endl;
                        }
                        break;

		case def:
			self = moveSet[index].getSelfChange();
                        if( self > 0 && tempStats[def] - baseStats[def] < 60)
                        {
                                tempStats[def] += self;
                                cout << getNickName() << "'s def rose." << endl;
                        }
                        else if(self > 0)
                        {
				tempStats[def] = baseStats[def] + 60;
                                cout << getNickName() << "'s def won't go higher." << endl;
                        }
                        else if( self < 0 && tempStats[def] + self >= 5 && baseStats[def] - tempStats[def] < 60)
                        {
                                tempStats[def] += self;
                                cout << getNickName() << "'s def fell." << endl;
                        }
                        else if( self < 0 )
                        {
                                cout << getNickName() << "'s def won't go lower." << endl;
                        }

                        foe = moveSet[index].getFoeChange();
                        if( foe > 0 && other.tempStats[def] - other.baseStats[def] < 60)
                        {
                                other.tempStats[def] += foe;
                                cout << other.getNickName() << "'s def rose." << endl;
                        }
                        else if(foe > 0)
                        {
				other.tempStats[def] = other.baseStats[def] + 60;
                                cout << other.getNickName() << "'s def won't go higher." << endl;
                        }
                        else if(foe<0&&other.tempStats[def]+foe>=5&&other.baseStats[def]-other.tempStats[def]<60)
                        {
                                other.tempStats[def] += foe;
                                cout << other.getNickName() << "'s def fell." << endl;
                        }
                        else if( foe < 0 )
                        {
                                cout << other.getNickName() << "'s def won't go lower." << endl;
                        }
                        break;

		case spDef:
			self = moveSet[index].getSelfChange();
                        if( self > 0 && tempStats[spDef] - baseStats[spDef] < 60)
                        {
                                tempStats[spDef] += self;
                                cout << getNickName() << "'s spDef rose." << endl;
                        }
                        else if(self > 0)
                        {
                                tempStats[spDef] = baseStats[spDef] + 60;
                                cout << getNickName() << "'s spDef won't go higher." << endl;
                        }
                        else if( self < 0 && tempStats[spDef] + self >= 5 && baseStats[spDef] - tempStats[spDef] < 60)
                        {
                                tempStats[spDef] += self;
                                cout << getNickName() << "'s spDef fell." << endl;
                        }
                        else if( self < 0 )
                        {
                                cout << getNickName() << "'s spDef won't go lower." << endl;
                        }

                        foe = moveSet[index].getFoeChange();
                        if( foe > 0 && other.tempStats[spDef] - other.baseStats[spDef] < 60)
                        {
                                other.tempStats[spDef] += foe;
                                cout << other.getNickName() << "'s dspDf rose." << endl;
                        }
                        else if(foe > 0)
                        {
                                other.tempStats[spDef] = other.baseStats[spDef] + 60;
                                cout << other.getNickName() << "'s spDef won't go higher." << endl;
                        }
                        else if(foe<0&&other.tempStats[spDef]+foe>=5&&other.baseStats[spDef]-other.tempStats[spDef]<60)
                        {
                                other.tempStats[spDef] += foe;
                                cout << other.getNickName() << "'s spDef fell." << endl;
                        }
                        else if( foe < 0 )
                        {
                                cout << other.getNickName() << "'s spDef won't go lower." << endl;
                        }
                        break;

		case spAtk:
			self = moveSet[index].getSelfChange();
                        if( self > 0 && tempStats[spAtk] - baseStats[spAtk] < 60)
                        {
                                tempStats[spAtk] += self;
                                cout << getNickName() << "'s spAtk rose." << endl;
                        }
                        else if(self > 0)
                        {
                                tempStats[spAtk] = baseStats[spAtk] + 60;
                                cout << getNickName() << "'s spAtk won't go higher." << endl;
                        }
                        else if( self < 0 && tempStats[spAtk] + self >= 5 && baseStats[spAtk] - tempStats[spAtk] < 60)
                        {
                                tempStats[spAtk] += self;
                                cout << getNickName() << "'s spAtk fell." << endl;
                        }
                        else if( self < 0 )
                        {
                                cout << getNickName() << "'s spAtk won't go lower." << endl;
                        }

                        foe = moveSet[index].getFoeChange();
                        if( foe > 0 && other.tempStats[spAtk] - other.baseStats[spAtk] < 60)
                        {
                                other.tempStats[spAtk] += foe;
                                cout << other.getNickName() << "'s spAtk rose." << endl;
                        }
                        else if(foe > 0)
                        {
                                other.tempStats[spAtk] = other.baseStats[spAtk] + 60;
                                cout << other.getNickName() << "'s spAtk won't go higher." << endl;
                        }
                        else if(foe<0&&other.tempStats[spAtk]+foe>=5&&other.baseStats[spAtk]-other.tempStats[spAtk]<60)
                        {
                                other.tempStats[spAtk] += foe;
                                cout << other.getNickName() << "'s spAtk fell." << endl;
                        }
                        else if( foe < 0 )
                        {
                                cout << other.getNickName() << "'s spAtk won't go lower." << endl;
                        }
                        break;
	}

}

//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
void Pokemon::moveHelpBurn(Pokemon &other)
{
	double dmg = getMaxHP()/10;
	reduceHP((int)(dmg));
	cout << getNickName() << " was hurt by the burn." << endl;
	
	//checks if ainted
	if(getHP() == 0)
	{
		cout << getNickName() << " has fainted." << endl;
		resetTemp();
	}
}
//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
void Pokemon::moveHelpPoison(Pokemon &other)
{
        double dmg = getMaxHP()/10;
        reduceHP((int)(dmg));
	cout << getNickName() << " was hurt by the poison." << endl;

	//cxhecks if fainted
	if(getHP() == 0)
	{
                cout << getNickName() << " has fainted." << endl;
		resetTemp();
	}
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

  //add onto baseStats using multipliers
  baseStats[maxHP] += mults[maxHP];
  baseStats[atk] += mults[atk];
  baseStats[def] += mults[def];
  baseStats[spAtk] += mults[spAtk];
  baseStats[spDef] += mults[spDef];
  baseStats[speed] += mults[speed];
  resetTemp();
}

void Pokemon::resetTemp()
{
	for(int i = 0; i < 7; i ++)
  	{
    		tempStats[i] = baseStats[i];
  	}
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

void Pokemon::gainHP(int h)
{
	baseStats[HP] += h;
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
  if(baseStats[HP] - lost <= 0)
  {
    baseStats[HP] = 0;
    effect = fainted;
  }
  else
    baseStats[HP] -= lost;
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
  return baseStats[HP];
}

int Pokemon:: getMaxHP()
{
  return baseStats[maxHP];
}

int Pokemon:: getAtk()
{
  return tempStats[atk];
}

int Pokemon:: getDef()
{
  return tempStats[def];
}

int Pokemon:: getSpAtk()
{
  return tempStats[spAtk];
}

int Pokemon:: getSpDef()
{
  return tempStats[spDef];
}

int Pokemon:: getSpeed()
{
  return tempStats[speed];
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
