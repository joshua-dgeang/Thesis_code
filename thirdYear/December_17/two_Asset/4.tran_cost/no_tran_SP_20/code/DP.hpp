//DP.hpp
#ifndef DP_HPP
#define DP_HPP
#include <fstream>
#include <vector>
using namespace std;
class DP {
	public: DP ();
		~DP();
		int ** NumPolicy;
		double *** PolicyTable;
	private:
		double ** OldV;
		double ** NewV;
		int ** OptPolicy_index;
		vector <double> cash_flow, cash_prob;
		const int maxC, maxA;
		const double smallNum, c_mean, c_std, discR, action_step_size;
		//Parameters used in initialization:
			double min_ini, max_ini;
			int counter_ini;
		//Functions:
		int Val_State_cash(double);
		int Val_State_asset(double);
		double State_Val_cash(int);
		double State_Val_asset(int);
		int FindAllAction(int,int,double*, double*);
			double cash_cur;
			double asset_cur;
			double A_cur, B_cur, C_cur, D_cur;
			int count_cur;
		void HermiteReader();
		void CalForOneStage();
			double best_value_s;
			int best_policy_index;
			double temp_Qvalue;
			double Qvalue(int, int, double);
				double cash_Q, asset_Q, current, future, sum;
				double cash_net;
			double interpolation(double, double);
				double VA, VB, VC, VD, VE, VF, VG;
				int xf, yf, xc, yc;
		bool Comparison();
			double maxdifference;
			double difference;
		void Repitition();
			int stage_counter;
			bool STP;
		void update();
		void TestOutPut();
			ofstream policy_OP;
			ofstream value_OP;
};
#endif
