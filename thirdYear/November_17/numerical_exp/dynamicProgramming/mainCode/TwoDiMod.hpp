//TwoDiMod.hpp
#ifndef TWODIMOD_HPP
#define TWODIMOD_HPP
#include <vector>
using namespace std;
class twodimod {
	public:
		twodimod();
		~twodimod();
	private:
		friend void TestOutput(twodimod &obj);
		double ** OldValue;
		double ** NewValue;
		int ** OptPolicy;
		int ** NumOfPolicy;
		int *** PolicyTable;
		bool ** Bankruptcy;
		vector<double> cash_demand;
		vector<double> cash_prob;
		//parameters used in Initialization;
			double cash_ini, asset_ini, A_ini, B_ini, C_ini, D_ini;
			int start_ini, end_ini, counter_ini;

		const int MaxC, MaxA;
		const double smallNum, c_mean, c_std, dicF;

		void HermiteReader();
		void CalForOneStage();
			double best_value_s;
			int best_policy_s;
			double Qvalue(int,int,int);
				double interplotation(double, double);
					double VA, VB, VC, VD, VE, VF, VG;
					int xf, yf, xc, yc;

		bool Comparison();
			double maxdifference;
			double difference;
		void Repetition();
			int stagecounter;
			bool STP;
		void update();
};
#endif
