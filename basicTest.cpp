//Tests basic functionality of pokemon class
//04/02/2015

#include <iostream>
#include "Pokemon.h"

int main()
{
  int stats[6];
  int muls[6];

  stats[HP] = 16;
  stats[maxHP] = 16;
  stats[atk] = 5;
  stats[def] = 10;
  stats[spAtk] = 7;
  stats[spDef] = 5;

  muls[HP] = 0;
  muls[maxHP] = 2;
  muls[atk] = 1;
  muls[def] = 2;
  muls[spAtk] = 1;
  muls[spDef] = 1;

  Pokemon blue(1, "Squirtle", "blue", 1, 10, stats, muls, normal, Ghost);
  blue.disp();
  cout << endl;

  blue.gainexp(5000);
  blue.changeStatus(asleep);
  blue.reduceHP(19);
  blue.disp();
 
    
  int val = blue.checkTyping(Normal);
  if(val == 1)
    cout << "Type: " <<  blue.getTypeText(Normal) << " is super effective." << endl;
  else if(val == 2)
    cout << "Type: " << blue.getTypeText(Normal) << " has no effect." << endl;
  else if(val == 0)
    cout << "Type: " << blue.getTypeText(Normal) << " has normal effect." << endl;
  else
    cout << "Type: " << blue.getTypeText(Normal) << " is not very effective." << endl;


  return 0;
}
