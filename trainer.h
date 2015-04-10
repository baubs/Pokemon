#ifndef TRAINER_H
#define TRAINER_H

using namespace std;

class Trainer {

	public:
		void pick_move;	//allows trainer to select move in battle
		void pick_pokemon;	//allows trainer to select pokemon in battle
		void poke_stats;	//allows trainer to check stats in battle
		Pokemon switch_pokemon(int m);	//allows trainer to switch order of pokemon


	private:
		Pokemon starter[6];
		vector<Pokemon> All_poke
		
};

#endif
