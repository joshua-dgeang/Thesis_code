//DP.hpp
#ifndef DP_HPP
#define DP_HPP
#include <fstream>
#include <vector>
using namespace std;
class DP {
	public: DP ();
		~DP();
	private:
		double ** OldV;
		double ** NewV;
		double ** OptP;
		double ** RP;
		double ** RV;
		double ** Comparison;
		bool ** loanAction;
		vector <double> cash_flow, cash_prob;
		const double maxC_value, maxA_value;
		int maxC, maxA;
		const double discrete, smallNum, c_mean, c_std, discR, action_step_size, returnRate, loanSize, loanInterest;//discrete: 1state = discrete * value. discrete should be between 0 to 1;
		const int loanTime;
		double loanPay;
		//Parameters used in initialization
			double min_ini, max_ini;
		//Functions:
		int Val_State_cash(double);
		int Val_State_asset(double);
		double State_Val_cash(int);
		double State_Val_asset(int);
		void FindAllAction(int,int,double*, double*, bool ToN, bool PoN);
			double cash_cur;
			double asset_cur;
			double A_cur, B_cur, C_cur, D_cur;
			int count_cur;
		void HermiteReader();
		void CalForOneStage(bool, bool);
			double best_value_s;
			double best_policy;
			double temp_Qvalue;
			double Qvalue(int, int, double, bool, bool);
				double cash_Q, asset_Q, current, future, sum;
				double cash_net;
			double interpolation(double, double);
				double VA, VB, VC, VD, VE, VF, VG;
				int scf, saf, scc, sac;// These are the state_of_cash_floor, state_of_asset_floor, state_of_cash_cel, state_of_asset_asset;
		void Repitition();
			int stage_counter;
			bool STP;
		void update();
		void TestOutPut();
			ofstream policy_OP;
			ofstream value_OP;
			ofstream loan;
				ofstream comp;
			ifstream ReadP;
			ifstream ReadV;
			char t;
};
#endif