//header.hpp
#ifndef HEADER_HPP
#define HEADER_HPP
#include <fstream>
class Draft {
	public: 
		Draft();
		~Draft();
		void test();
	private:
		void sarsa_lambda();
			int S_x, S_y, A, S_x_next, S_y_next, A_next;
			bool bankruptcy, terminal;
			double cash_os, asset_os, action_os,tranCst, cash_demand, cost_by_shortage;
			double currentReward;
			double DELTA;
			bool Repeat;
		double gaussrand();
		void EndOfEpisode();
		void RepeatEpisode();
			int EpisodeCounter;
			const int maximumEpisode;
		typedef struct stateValue{
			double Qvalue;
			double Zvalue;
			int CashAction;
			stateValue* next;
		}*statePointer;

		statePointer head, curr, temp;

		statePointer ** threeDptr;


		const int Max_C, Max_A;
		const double ReturnRate, TF_I, TF_II,TP_I,TP_II, CD_mean, CD_std, ShorPen, discR;
		const double  ALPHA, EPSILON, LAMBDA;
		const int start_cash, start_asset;
		//Variables used in finding NumOfAction[i][j];
		double cash_ini, asset_ini, A_ini, B_ini, C_ini, D_ini;
		int start_ini, end_ini, counter_ini;

		std::ofstream Result;
		std::ifstream Policy;
			char t;
		void fileReader();

};
#endif
