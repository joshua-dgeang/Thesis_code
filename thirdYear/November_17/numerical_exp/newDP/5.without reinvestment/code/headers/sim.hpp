//sim.hpp
//this class is used to simulate cash problem for one stage;
//users shall pass two double pointers to the current cash and asset level and two constant parameters (cash demand, cash action);
//this class will return the immidiate reward for this stage as well as modify the two pointers (cash and asset)

#ifndef SIM_HPP
#define SIM_HPP	
class sim {
	public:
		sim (double * const, double * const, const double, const double);
		double getCurrentReward ();
		static const double ReturnRate, TF_I, TF_II, TP_I, TP_II, ShorPen, discR;

	private:
		double currentReward;
		
		double * cashLevel, * const assetLevel;
		double cashInflow;

		const double cash_demand, cash_action;

		void simulation();
			double cost_by_trans;
			double cost_by_shortage;
            double cost_by_cash;
};
#endif
