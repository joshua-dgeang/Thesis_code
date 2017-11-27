//one_Asset.hpp
#ifndef ONE_ASSET_HPP
#define ONE_ASSET_HPP
#include "Hermite.cpp"
class one_Asset{
	public:
		one_Asset();
		~one_Asset();
		double ** PolicyTable;
		int * totalNumber_of_Actions;

	private:
		static const double ReturnRate, TF_I, TF_II, TP_I, TP_II, SCR, discR, smallNum, c_mean, c_std, Action_size;
		static const int MaxC;
		double * OldV;
		double * NewV;
		int * OptAction_index;

		vector<double> cash_demand, cash_prob;
		//functions:
		void ReadHermite ();
		double simulation(double*, double, int);//pass a pointer to cash/state, a double parameter:cash demand, and a int parameter:policy index;
			double Action_value;
		double TranCost_I(double);//Transaction cost of moving money from cash to asset;
		double TranCost_II(double);//Transaction cost of moving money from asset to cash;
		double State_to_Value(int);
		int Value_to_State(double);

};
#endif
