//Brandon Aubrey, Morris LaGrand, Tyler Sammons
//Base class for all Pokemon
//10/25/2015

#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <string>
//#include "move.h"
using namespace std;

//enumeration for current status of the pokemon
enum status {normal, burned, poisoned, asleep, paralyzed, frozen, fainted};
//enumeration for indexes of stat array
enum stats {HP, maxHP, atk, def, spAtk, spDef};

class Pokemon
{
  public:
    Pokemon(); //default constructor
    Pokemon(string, string, int, int, int[6], int[6], status);//, int[4]); //nondefault constr
    void levelup();
    void gainexp(int);
    void changeStatus(status);
    void reduceHP(int);
    status getStatus();
    int getLevel();
    int getExp();
    int getMaxExp();
    int getHP();
    int getMaxHP();
    int getAtk();
    int getDef();
    int getSpAtk();
    int getSpDef();
    string getName();
    string getNickName();
    void disp();
  private:
    string name;
    string nickname;
    int level;
    int exp;  
    int nxtLev;  //exp needed to gain level
    int stats[6];
    int mults[6]; //multipliers for each stat
    status effect;
   // move moveSet[4];  
};
#endif
