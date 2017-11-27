//TwoD.hpp
#ifndef TWOD_HPP
#define TWOD_HPP
#include <fstream>
#include <vector>
using namespace std;
class TwoD {
	public: TwoD();
		~TwoD();
		int **NumPolicy;//total number of feasible actions given a specific state;
		double *** PolicyTable;//first two dimensions describe the state, the third dimension gives the index of the policy, this table record the value of the corresponding 
	private:
		double **OldValue;
		double **NewValue;
		int **OptPolicy;//record the index of the best action.
		vector<double>cash_demand, cash_prob;

		const int MaxC, MaxA;
		const double smallNum, c_mean, c_std, discR, action_step_size;

		//parameters used in initialization:
			double cash_ini, asset_ini, A_ini, B_ini, C_ini, D_ini;
			int start_ini, end_ini, counter_ini;

		void HermiteReader();
		void CalForOneStage();
			double best_value_s;
			int best_policy_index;
			double Qvalue(int, int, double);
				double interpolation(double, double);
					double VA, VB, VC, VD, VE, VF, VG;
					int xf, yf, xc, yc;
				
		bool Comparison ();
			double maxdifference;
			double difference;
		void Repetition();
			int stagecounter;
			bool STP;
		void update();

		int ValtoState_cash(double);
		int ValtoState_asset(double);
		double StatetoVal_cash(int);
		double StatetoVal_asset(int);

		void TestOutPut();
			ofstream valueoutput;
			ofstream policyoutput;
		



};
#endif
