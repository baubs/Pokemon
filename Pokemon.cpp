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
  mults[HP] = 0;      //do not want to add on to current HP
  mults[maxHP] = 2;
  mults[atk] = 1;
  mults[def] = 1;
  mults[spAtk] = 1;
  mults[spDef] = 1;
  effect = normal;
}


//non default constructor 
Pokemon:: Pokemon(int num, string n, string nn, int lev, int nxtLevel,int stat[6], int muls[6],status eff, types t)
{
  number = num;
  name = n;
  nickname = nn;
  level = lev;
  exp = 0;
  nxtLev = nxtLevel;
  type = t;
  
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
      toRet = "Frozen";
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
  cout << "HP: " << getHP() << "/" << getMaxHP() << endl;
  cout << "Atk: " << getAtk() << "\t" << "Def: " << getDef() << endl;
  cout << "SpA: " << getSpAtk() << "\t" << "SpD: " << getSpDef() << endl;
}

//checks typing, 2 = no effect; 1 = super; 0 = normal; -1 = resistant 
int Pokemon::checkTyping(types t)
{
  switch(type)
  {
    case Normal:
      if(t == Fight)
        return 1;
      else if(t == Ghost)
        return 2;
      else
        return 0;
      break;

    case Grass:
      if(t == Fire || t == Ice || t == Poison || t == Flying)
        return 1;
      else if(t == Water || t == Eletric || t == Grass || t == Ground)
        return -1;
      else
        return 0;
      break;

    case Water:
      if(t == Eletric || t == Grass)
        return 1;
      else if(t == Fire || t == Water || t == Ice)
        return -1;
      else
        return 0;
      break;

    case Fire:
      if(t == Water || t == Ground || t == Rock)
        return 1;
      else if(t == Fire || t == Grass || t == Ice || t == Bug)
        return -1;
      else
        return 0;
      break;

    case Flying:
      if(t == Eletric || t == Rock || t == Ice)
        return 1;
      else if(t == Fight || t == Grass ||  t == Bug)
        return -1;
      else if(t == Ghost)
        return 2;
      else
        return 0;
      break;

    case Fight:
      if(t == Flying || t == Psychic)
        return 1;
      else if(t == Rock || t == Dark || t == Bug)
        return -1;
      else
        return 0;
      break;

    case Psychic:
      if(t == Bug || t == Ghost || t == Dark)
        return 1;
      else if(t == Psychic || t == Fight)
        return -1;
      else
        return 0;
      break;

    case Bug:
      if(t == Fire || t == Flying || t == Rock)
        return 1;
      else if(t == Fight || t == Grass || t == Ground)
        return -1;
      else
        return 0;
      break;

    case Poison:
      if(t == Ground || t == Psychic)
        return 1;
      else if(t == Bug || t == Grass || t == Fight || t == Poison)
        return -1;
      else
        return 0;
      break;

    case Eletric:
      if(t == Ground)
        return 1;
      else if(t == Eletric || t == Flying)
        return -1;
      else
        return 0;
      break;

    case Rock:
      if(t == Water || t == Ground || t == Fight | Grass)
        return 1;
      else if(t == Normal || t == Fire || t == Poison || t == Flying)
        return -1;
      else
        return 0;
      break;

    case Ground:
      if(t == Water || t == Grass || t == Ice)
        return 1;
      else if(t == Rock || t == Poison)
        return -1;
      else if(t == Eletric)
        return 2;
      else
        return 0;
      break;

    case Ghost:
      if(t == Ghost || t == Dark)
        return 1;
      else if(t == Poison || t == Bug)
        return -1;
      else if(t == Normal || t == Fight)
        return 2;
      else
        return 0;
      break;

    case Dark:
      if(t == Fight || t == Bug)
        return 1;
      else if(t == Ghost || t == Dark)
        return -1;
      else if(t == Psychic)
        return 2;
      else
        return 0;
      break;

    case Ice:
      if(t == Fire || t == Fight || t == Rock)
        return 1;
      else if(t == Ice)
        return -1;
      else
        return 0;
      break;

    case Dragon:
      if(t == Ice || t == Dragon)
        return 1;
      else if(t == Fire || t == Grass || t == Water || t == Eletric)
        return -1;
      else
        return 0;
      break;
  }
}



