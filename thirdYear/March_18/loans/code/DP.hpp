//DP.hpp
#ifndef DP_HPP
#define DP_HPP
#include <fstream>
#include <vector>
using namespace std;
class DP {
	public: DP ();
		~DP ();

	private:
		double *** OldV;
		double *** NewV;
		int *** NumPolicy;//record the total number of feasible actions given current state;
		int *** OptPolicy_index;//record the INDEX of the optimal cash action give the current state ***;
		double **** PolicyTable;//first three dimensions give the current state, the last dimension gives the index of actions;
		bool ** LoanAction;//Only applied to states where taking loans is optional, 1 is taking the loan, 0 is not taking any loan.
		const double maxC_value, maxA_value, discrete, smallNum, c_mean, c_std, discR, action_step_size, returnRate, loanSize, loanRate;
		double loanPayment;
		const int maxLoanTime;
		int maxC, maxA;
		int Val_State_cash(double value, int maxState);
		double State_Val(int state);
			int tempState;
		vector <double> cash_flow, cash_prob;
		//parameters used in Initalisation II...
			double min_ini, max_ini;
			int counter_ini;

		int FindAllAction(int z, int x, int y, double *min, double *max);
			double cash_cur;
			double asset_cur;
			double A_cur, B_cur, C_cur, D_cur;
			int count_cur;
};

#endif
