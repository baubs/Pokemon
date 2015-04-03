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

  Pokemon blue(1, "Squirtle", "blue", 1, 10, stats, muls, normal);
  blue.disp();
  cout << endl;

  blue.gainexp(5000);
  blue.changeStatus(asleep);
  blue.reduceHP(19);
  blue.disp();
 
  return 0;
}
