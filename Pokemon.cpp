//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//Base class for all Pokemon
//03/25/2015

#include <iostream>
#include <string>
#include "Pokemon.h"
//#include "move.h"
using namespace std;

Pokemon:: Pokemon()
{
  name = "Pikachu";
  nickname = "Pika";
  level = 1;
  exp = 0;
  nxtLev = 50;
  stats[HP] = 20;
  stats[maxHP] = stats[HP];
  stats[atk] = 1;
  stats[def] = 1;
  stats[spAtk] = 1;
  stats[spDef] = 1;
  mults[HP] = 0;      //do not want to add on to current HP
  mults[maxHP] = 2;
  mults[atk] = 1;
  mults[def] = 1;
  mults[spAtk] = 1;
  mults[spDef] = 1;
  effect = normal;
}


//non default constructor 
Pokemon:: Pokemon(string n, string nn, int lev, int nxtLevel, int stat[6], int muls[6], status eff)
{
  name = n;
  nickname = nn;
  level = lev;
  exp = 0;
  nxtLev = nxtLevel;
  
  for(int i = 0; i < 6; i ++)
  {
    stats[i] = stat[i];
    mults[i] = muls[i];
  }

  effect = eff;
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
}

//giving a pokmon exp
void Pokemon:: gainexp(int xp)
{
  //if pokemon gains more exp than it needs to 
  if(exp + xp >= nxtLev)
  {
    levelup();                       
    gainexp(exp + xp - nxtLev);   //spill over exp
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

//Display function
void Pokemon::disp()
{
  cout << getName() << ", \"" << getNickName() << "\" Level: " << getLevel() << endl;
  cout << "EXP: " << getExp() << "/" << getMaxExp() << endl;
  cout << "HP: " << getHP() << "/" << getMaxHP() << endl;
  cout << "Atk: " << getAtk() << "\t" << "Def: " << getDef() << endl;
  cout << "SpAtk: " << getSpAtk() << "\t" << "SpDef: " << getSpDef() << endl;
}









