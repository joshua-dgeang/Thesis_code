#ifndef SARSAL_HPP
#define SARSAL_HPP
#include "simu.cpp"
#include <fstream>
class SarsaL{
	public:
		SarsaL();
		~SarsaL();
	private:
		int cash, asset, action;
		double demand;
		bool stop;
		double gaussrand();
			const double CD_mean, CD_std;
		const int Max_C, Max_A;

		typedef struct stateValue{
			double Qvalue;
			double Zvalue;
			int OptimalAction;
			stateValue* next;
		}*statePointer;

		statePointer head,curr;
		statePointer ** twoDptr;

		void fileReader(); 
		ifstream Policy;
		ofstream test;
			char t;
		void RepeatEpisode();
			int EpisodeCounter;
			int MaxEpisode;
				int S_x, S_y;
				bool stp;
		void sarsa_lambda(int , int, bool);
			const double ALPHA, disC, LAMBDA;
			double DELTA;
			bool repeat;
			void EndofEpisode();
		ofstream output;

			
};
#endif
