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
enum stats {HP, maxHP, atk, def, spAtk, spDef, speed};

//enumeration for types of move/pokemon
enum types {Normal, Grass, Water, Fire, Flying, Fight, Psychic, Bug, Poison, Eletric, Rock, Ground, Ghost, Dark, Ice, Dragon};

class Pokemon
{
  public:
    Pokemon(); //default constructor
    Pokemon(int, string, string, int, int, int[7], int[7], status, types);//, int[4]); //nondefault constr
    Pokemon(int); //loads Pokemon from Pokelist using the number (1-150)
    void typeFromText(string);  //sets the type based on string
    void levelup();
    void gainexp(int);
    void changeStatus(status);
    void reduceHP(int);
    status getStatus();
    types getType();
    int getLevel();
    int getExp();
    int getMaxExp();
    int getHP();
    int getMaxHP();
    int getAtk();
    int getDef();
    int getSpAtk();
    int getSpDef();
    int getSpeed();
    int checkTyping(types);       //compares type to pokemon type, returns 1=effective, 0=normal, -1=resistant
    string getName();
    string getNickName();
    string getStatusText();      // returns the status as a string
    string getTypeText(types);   //returns the type as a string
    int getNumber();
    void disp();
  private:
    int number;
    string name;
    string nickname;
    types type;
    int level;
    int exp;  
    int nxtLev;  //exp needed to gain level
    int stats[7];
    int mults[7]; //multipliers for each stat
    status effect;
   // move moveSet[4];  
};
#endif
