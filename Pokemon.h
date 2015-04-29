 //Brandon Aubrey, Morris LaGrand, Tyler Sammons
//Base class for all Pokemon
//10/25/2015

#ifndef POKEMON_H
#define POKEMON_H

#include <vector>
#include <iostream>
#include <string>
#include "move.h"
using namespace std;


class Pokemon
{
  public:
    Pokemon(); 				//default constructor
    ~Pokemon(); 			//default destructor
    Pokemon(int, string, string, int, int, int[7], int[7], status, types);//, int[4]); //nondefault constr
    Pokemon(int); 			//loads Pokemon from Pokelist using the number (1-150)
    void typeFromText(string);  	//sets the type based on string
    void levelup();			//promotes pokemon to next level
    void gainexp(unsigned long int);			//give pokemon specified exp and levels up if needed 
    void changeStatus(status);		//changes the status of the pokemon
    void reduceHP(int);			//used in useMove to update health
    void gainHP(int);			//used in useMove to deal with special case moves
    status getStatus();			//returns enum "current status"
    types getType();			//returns enum "type"
    void resetTemp();			//returns temp stats to base stats after battle ends
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
    string battleDisp();			//displays limited info for battle sceens
    string disp_move(int);   		//displays just one move
    vector<string> disp_moves();     		//displays whole move set
    void LoadMoves(int, int);    	//loads move from moveList file to movelist member using index
    string useMove(int, Pokemon &);   	//uses move index on pokemon
    string moveHelpBurn(Pokemon &);            	//HELPER FUNCTIONS HANDLING STATUSES IN USEMOVE
    string moveHelpPoison(Pokemon &);		//^
    string moveHelpAsleepFrozen();	//^^
    string statsChange(int, int, Pokemon &);			//^^^ but with stats
    unsigned long int giveXP();
    void savePokemon();			//saves pokemon to saveFile.txt
  private:
    int number;
    string name;
    string nickname;
    types type;
    int level;
    unsigned long int exp;  
    int nxtLev;  //exp needed to gain level
    int baseStats[7];
    int tempStats[7];
    int mults[7]; //multipliers for each stat
    status effect;
    move *moveSet;  
};
#endif
