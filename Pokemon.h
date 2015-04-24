 //Brandon Aubrey, Morris LaGrand, Tyler Sammons
//Base class for all Pokemon
//10/25/2015

#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <string>
#include "move.h"
using namespace std;

//enumeration for current status of the pokemon
//enum status {normal, burned, poisoned, asleep, paralyzed, frozen, fainted};
//enumeration for indexes of stat array
//enum stats {HP, maxHP, atk, def, spAtk, spDef, speed};

//enumeration for types of move/pokemon
//enum types {Normal, Grass, Water, Fire, Flying, Fight, Psychic, Bug, Poison, Eletric, Rock, Ground, Ghost, Dark, Ice, Dragon};

class Pokemon
{
  public:
    Pokemon(); 				//default constructor
    ~Pokemon(); 			//default destructor
    Pokemon(int, string, string, int, int, int[7], int[7], status, types);//, int[4]); //nondefault constr
    Pokemon(int); 			//loads Pokemon from Pokelist using the number (1-150)
    void typeFromText(string);  	//sets the type based on string
    void levelup();			//promotes pokemon to next level
    void gainexp(int);			//give pokemon specified exp and levels up if needed 
    void changeStatus(status);		//changes the status of the pokemon
    void reduceHP(int);			//used in useMove to update health
    status getStatus();			//returns enum "current status"
    types getType();			//returns enum "type"
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
    int checkTyping(types);       	//compares type to pokemon type and returns power multiplier
    string getName();
    string getNickName();
    string getStatusText();      	// returns the status as a string
    string getTypeText(types);   	//returns the type as a string
    int getNumber();
    void disp();           		//displays the Pokemons current state and stats 
    void battleDisp();			//displays limited info for battle sceens
    void disp_move(int);   		//displays just one move
    void disp_moves();     		//displays whole move set
    void LoadMoves(int, int);    	//loads move from moveList file to movelist member using index
    void useMove(int, Pokemon &);   	//uses move index on pokemon
    void moveHelpBurn();            	//HELPER FUNCTIONS HANDLING STATUSES IN USEMOVE
    void moveHelpPoison();		//^
    void moveHelpAsleepFrozen();	//^^
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
    move *moveSet;  
};
#endif
