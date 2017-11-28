//one_Asset.hpp
#ifndef ONE_ASSET_HPP
#define ONE_ASSET_HPP
#include "Hermite.cpp"
#include <fstream>
class one_Asset{
	public:
		one_Asset();
		~one_Asset();
		double simulation(double*, double, double);//pass a pointer to cash/state, a double parameter:cash net inflow, and a int parameter:policy value;

	private:
		static const double ReturnRate, TF_I, TF_II, TP_I, TP_II, SCR, discR, smallNum, c_mean, c_std, Action_size;
		static const int MaxC;
		double * OldV;
		double * NewV;
		int * OptAction_index;
		int * totalNumber_of_Actions;
		double ** PolicyTable;

		vector<double> cash_demand, cash_prob;
		//functions:
		void ReadHermite ();
			double cost_by_transaction;
			double cost_by_shortage;
			double cost_by_holding;
		void Repetition();
			int stagecounter;
			bool STP;
		void CalForOneStage();
			double best_value_s;
			int best_policy_index;
			double Qvalue(int, double);
			double interpolation(double);
				int xf, xc;
		bool Comparison();
			double maxdifference;
			double difference;
		void update();
		double TranCost(double);//Transaction cost of moving money;
		double State_to_Value(int);
		int Value_to_State(double);
		void OutPut();
			ofstream valueoutput;
			ofstream policyoutput;
			ofstream logJournal;
	

};
#endif
