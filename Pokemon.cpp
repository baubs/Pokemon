//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//Base class for all Pokemon
//03/25/2015

#include <vector>
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
    ss >> mults[maxHP] >> mults[atk] >> mults[def] >> mults[spAtk] >> mults[spDef] >> mults[speed]; 

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
string Pokemon::useMove(int index, Pokemon &other)
{
    string str1;
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
		str1 += getNickName() + " is paralyzed! It can't move!";

	//handleing frozen and asleep pokemon
	else if(getStatus() == asleep || getStatus() == frozen )
		str1 += moveHelpAsleepFrozen();

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
		str1 += " " + getNickName() + " used " + moveSet[index].getName();
		if(typeEffect == 10 || rounded == 0)
			str1 += ".";
		else if(typeEffect == 20)
			str1 += ", its super effective.";
		else if(typeEffect == 5)
            str1 += ", its not very effective.";
		else if(typeEffect == 0)
			str1 += ", it has no effect on foe " + other.getNickName() + ".";
		else
			str1 += ", and it is a broken move.";
		
		//passes on status effect to other pokemon
		int statusProc = rand()%100;
		if(statusProc < moveSet[index].get_prob() && moveSet[index].get_status() != normal)
		{
			if(other.getStatus() == normal)
			{
				status change = moveSet[index].get_status();
				other.changeStatus(change);
				str1 += " " + other.getNickName() + " is " + other.getStatusText();
			}
			else
				str1 += "it had no effect on foe's status.";
		}
		//update other pokemon's hp and moves pp
		other.reduceHP(rounded);
		moveSet[index].reduce_pp();
		str1 += " " + statsChange(index, rounded,  other);
	}
	//move missed
	else
	{	
		str1 += " " + getNickName() + " used " + moveSet[index].getName() + ", it missed.";
		moveSet[index].reduce_pp();
	}

	//burn and poison damages
	if(getStatus() == burned)
		str1 += " " + moveHelpBurn(other);
	else if(getStatus() == poisoned)
		str1 += " " + moveHelpPoison(other);

	//checks if move KOed other pokemon
	if(other.getHP() == 0)
	{
		string xp;
		stringstream convertxp;
		
		str1 += " " + other.getNickName() + " fainted.";
		other.resetTemp();
		resetTemp();
		gainexp(other.giveXP());
		convertxp << other.giveXP();
		convertxp >> xp;
		str1 += getNickName() + " gained " + xp + " exp. "; 
	}
    return str1;
}

string Pokemon::statsChange(int index, int damageDone, Pokemon &other)
{
    //for HP stat
    string toRet;
    switch( moveSet[index].getChangeStat() )
    {
            int self, foe;
            
        case HP:
            if( moveSet[index].getSelfChange() == 5 )
            {
                gainHP(damageDone/2);
                toRet += getNickName() + " absorbed health.";
            }
            else if( moveSet[index].getSelfChange() < 0 )
            {
                gainHP(moveSet[index].getSelfChange());
                toRet += getNickName() + " was hit by recoil.";
                if(getHP() == 0)
                    toRet += getNickName() + " has fainted.";
            }
            else if( moveSet[index].getSelfChange() > 0)
            {
                gainHP(moveSet[index].getSelfChange());
                toRet += getNickName() + " recovered health.";
            }
            other.gainHP(moveSet[index].getFoeChange());
            break;
        case speed:
            self = moveSet[index].getSelfChange();
            if( self > 0 && tempStats[speed] - baseStats[speed] < 60)
            {
                tempStats[speed] += self;
                toRet += getNickName() + "'s speed rose.";
            }
            else if(self > 0)
            {
                toRet += getNickName() + "'s speed won't go higher.";
            }
            else if( self < 0 && tempStats[speed] + self >= 5)
            {
                tempStats[speed] += self;
                toRet += getNickName() + "'s speed fell.";
            }
            else if( self < 0 )
            {
                toRet += getNickName() + "'s speed won't go lower.";
            }
            
            foe = moveSet[index].getFoeChange();
            if( foe > 0 && other.tempStats[speed] - other.baseStats[speed] < 60)
            {
                other.tempStats[speed] += foe;
                toRet += other.getNickName() + "'s speed rose.";
            }
            else if(foe > 0)
            {
                toRet += other.getNickName() + "'s speed won't go higher.";
            }
            else if( foe < 0 && other.tempStats[speed] + foe >= 5)
            {
                other.tempStats[speed] += foe;
                toRet += other.getNickName() + "'s speed fell.";
            }
            else if( foe < 0 )
            {
                toRet += other.getNickName() + "'s speed won't go lower.";
            }
            break;
            
        case atk:
            self = moveSet[index].getSelfChange();
            if( self > 0 && tempStats[atk] - baseStats[atk] < 60)
            {
                tempStats[atk] += self;
                toRet += getNickName() + "'s atk rose.";
            }
            else if(self > 0)
            {
                tempStats[atk] = baseStats[atk] + 60;
                toRet += getNickName() + "'s atk won't go higher.";
            }
            else if( self < 0 && tempStats[atk] + self >= 5)
            {
                tempStats[atk] += self;
                toRet += getNickName() + "'s atk fell.";
            }
            else if( self < 0 )
            {
                toRet += getNickName() + "'s atk won't go lower.";
            }
            
            foe = moveSet[index].getFoeChange();
            if( foe > 0 && other.tempStats[atk] - other.baseStats[atk] < 60)
            {
                other.tempStats[atk] += foe;
                toRet += other.getNickName() + "'s atk rose.";
            }
            else if(foe > 0)
            {
                other.tempStats[atk] = other.baseStats[atk] + 60;
                toRet += other.getNickName() + "'s atk won't go higher.";
            }
            else if( foe < 0 && other.tempStats[atk] + foe >= 5)
            {
                other.tempStats[atk] += foe;
                toRet += other.getNickName() + "'s atk fell.";
            }
            else if( foe < 0 )
            {
                toRet += other.getNickName() + "'s atk won't go lower.";
            }
            break;
            
        case def:
            self = moveSet[index].getSelfChange();
            if( self > 0 && tempStats[def] - baseStats[def] < 60)
            {
                tempStats[def] += self;
                toRet += getNickName() + "'s def rose.";
            }
            else if(self > 0)
            {
                tempStats[def] = baseStats[def] + 60;
                toRet += getNickName() + "'s def won't go higher.";
            }
            else if( self < 0 && tempStats[def] + self >= 5 && baseStats[def] - tempStats[def] < 60)
            {
                tempStats[def] += self;
                toRet += getNickName() + "'s def fell.";
            }
            else if( self < 0 )
            {
                toRet += getNickName() + "'s def won't go lower.";
            }
            
            foe = moveSet[index].getFoeChange();
            if( foe > 0 && other.tempStats[def] - other.baseStats[def] < 60)
            {
                other.tempStats[def] += foe;
                toRet += other.getNickName() + "'s def rose.";
            }
            else if(foe > 0)
            {
                other.tempStats[def] = other.baseStats[def] + 60;
                toRet += other.getNickName() + "'s def won't go higher.";
            }
            else if(foe<0&&other.tempStats[def]+foe>=5&&other.baseStats[def]-other.tempStats[def]<60)
            {
                other.tempStats[def] += foe;
                toRet += other.getNickName() + "'s def fell.";
            }
            else if( foe < 0 )
            {
                toRet += other.getNickName() + "'s def won't go lower.";
            }
            break;
            
        case spDef:
            self = moveSet[index].getSelfChange();
            if( self > 0 && tempStats[spDef] - baseStats[spDef] < 60)
            {
                tempStats[spDef] += self;
                toRet += getNickName() + "'s spDef rose.";
            }
            else if(self > 0)
            {
                tempStats[spDef] = baseStats[spDef] + 60;
                toRet += getNickName() + "'s spDef won't go higher.";
            }
            else if( self < 0 && tempStats[spDef] + self >= 5 && baseStats[spDef] - tempStats[spDef] < 60)
            {
                tempStats[spDef] += self;
                toRet += getNickName() + "'s spDef fell.";
            }
            else if( self < 0 )
            {
                toRet += getNickName() + "'s spDef won't go lower.";
            }
            
            foe = moveSet[index].getFoeChange();
            if( foe > 0 && other.tempStats[spDef] - other.baseStats[spDef] < 60)
            {
                other.tempStats[spDef] += foe;
                toRet += other.getNickName() + "'s dspDf rose.";
            }
            else if(foe > 0)
            {
                other.tempStats[spDef] = other.baseStats[spDef] + 60;
                toRet += other.getNickName() + "'s spDef won't go higher.";
            }
            else if(foe<0&&other.tempStats[spDef]+foe>=5&&other.baseStats[spDef]-other.tempStats[spDef]<60)
            {
                other.tempStats[spDef] += foe;
                toRet += other.getNickName() + "'s spDef fell.";
            }
            else if( foe < 0 )
            {
                toRet += other.getNickName() + "'s spDef won't go lower.";
            }
            break;
            
        case spAtk:
            self = moveSet[index].getSelfChange();
            if( self > 0 && tempStats[spAtk] - baseStats[spAtk] < 60)
            {
                tempStats[spAtk] += self;
                toRet += getNickName() + "'s spAtk rose.";
            }
            else if(self > 0)
            {
                tempStats[spAtk] = baseStats[spAtk] + 60;
                toRet += getNickName() + "'s spAtk won't go higher.";
            }
            else if( self < 0 && tempStats[spAtk] + self >= 5 && baseStats[spAtk] - tempStats[spAtk] < 60)
            {
                tempStats[spAtk] += self;
                toRet += getNickName() + "'s spAtk fell.";
            }
            else if( self < 0 )
            {
                toRet += getNickName() + "'s spAtk won't go lower.";
            }
            
            foe = moveSet[index].getFoeChange();
            if( foe > 0 && other.tempStats[spAtk] - other.baseStats[spAtk] < 60)
            {
                other.tempStats[spAtk] += foe;
                toRet += other.getNickName() + "'s spAtk rose.";
            }
            else if(foe > 0)
            {
                other.tempStats[spAtk] = other.baseStats[spAtk] + 60;
                toRet += other.getNickName() + "'s spAtk won't go higher.";
            }
            else if(foe<0&&other.tempStats[spAtk]+foe>=5&&other.baseStats[spAtk]-other.tempStats[spAtk]<60)
            {
                other.tempStats[spAtk] += foe;
                toRet += other.getNickName() + "'s spAtk fell.";
            }
            else if( foe < 0 )
            {
                toRet += other.getNickName() + "'s spAtk won't go lower.";
            }
            break;
    }
    
    return toRet;
    
}


//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
string Pokemon::moveHelpBurn(Pokemon &other)
{
    string helpBurn;
    double dmg = getMaxHP()/10;
	reduceHP((int)(dmg));
	helpBurn += getNickName() + " was hurt by the burn.";
	
	//checks if ainted
	if(getHP() == 0)
	{
		helpBurn += getNickName() + " has fainted.";
		resetTemp();
	}
    return helpBurn;
}
//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
string Pokemon::moveHelpPoison(Pokemon &other)
{
    string helpPoison;
    double dmg = getMaxHP()/10;
    reduceHP((int)(dmg));
	helpPoison += getNickName() + " was hurt by the poison.";

	//cxhecks if fainted
	if(getHP() == 0)
	{
        helpPoison += getNickName() + " has fainted.";
		resetTemp();
	}
    return helpPoison;
}
//USEMOVE HELPER FUNCTIONS FOR STATUS EFFECTS ON POKEMON USING THE MOVE
string Pokemon::moveHelpAsleepFrozen()
{
    string helpFrozen;
	int check = rand()%2;

	if(check == 1 && getStatus() == asleep)
	{
		changeStatus(normal);
		helpFrozen += getNickName() + " has woken up.";
	}
	else if(check)
        {
                changeStatus(normal);
                helpFrozen += getNickName() + " has thawed.";
        }
	else if(getStatus() == asleep)
		helpFrozen += getNickName() + " is fast asleep.";
	else
	  helpFrozen += getNickName() + " is frozen solid.";
    
    return helpFrozen;
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
void Pokemon:: gainexp(unsigned long int xp)
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
	if(baseStats[HP] + h > baseStats[maxHP])
		baseStats[HP] = baseStats[maxHP];
	else
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
string Pokemon::battleDisp()
{
    string y;
    string str;
    string level;
    string HP;
    string maxHP;
    stringstream convertLev;
    stringstream convertHP;
    stringstream convertmaxHP;
    
    if(getStatus() != normal)
        y = " " + getStatusText(); //+ endl;
    else
        y = "";
    
    convertLev << getLevel();
    convertLev >> level;
    convertHP << getHP();
    convertHP >> HP;
    convertmaxHP << getMaxHP();
    convertmaxHP >> maxHP;
    str = getNickName() + " : lvl " + level + " HP: " + HP + "/" + maxHP + y;

    return str;
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
string Pokemon::disp_move(int i) {

  string str;
  string x;
  stringstream convertint;
  convertint << i;
  convertint >> x;

  str = x;
  str += ": ";
  str += moveSet[i].BattleDisplay();
  return str;
}

//displays the whole moveset
vector<string> Pokemon::disp_moves()
{
  vector<string> moves;

  moves.push_back(disp_move(0));
  moves.push_back(" || ");
  moves.push_back(disp_move(1));
  moves.push_back(disp_move(2));
  moves.push_back(" || ");
  moves.push_back(disp_move(3));

  return moves;
}

//determines amount of xp to award killer
unsigned long int Pokemon::giveXP()
{
	return (15*level*level)+(50*level)+50;
}

//saves pokemon to filename
void Pokemon::savePokemon()
{
	ofstream myfile;
	myfile.open("savefile.txt");
	myfile << getName() << "\t" << getTypeText(getType()) << "\t" << getMaxHP() << "\t" << getAtk() << endl;
	myfile.close();
}
