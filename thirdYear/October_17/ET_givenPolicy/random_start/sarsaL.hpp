#ifndef SARSAL_HPP
#define SARSAL_HPP
#include "simu.cpp"
#include <fstream>
class sarsaL{
	public:
		sarsaL();
		~sarsaL();
	private:
		//Functions:
		void fileReader ();
			ifstream Policy;
			ofstream Reco;
			ofstream Record;
			char t;
		double gaussrand();
		void Repeat ();
			int EpisodeCounter, MaxEpisode;
			int stateX, stateY;
			bool stp;
		void sarsa_lambda();
			double DELTA;
			bool repeat;
			int old_sx, old_sy;
			double demand;
		void AddNewNode(bool);
		void EndOfEpisode();

			
		//Global variables:
		const double CD_mean, CD_std;
		const int Max_C, Max_A;
		const double alpha, discR, lambda;
		typedef struct linkedList {
			int S_x, S_y;
			double Qvalue;
			double Zvalue;
			int OptimalAction;
			linkedList* next;
		} * LinkPtr;
		LinkPtr head, curr;

		double ** Vvalue;
		int ** OptAct;
		
		


	      	
	


};
#endif
