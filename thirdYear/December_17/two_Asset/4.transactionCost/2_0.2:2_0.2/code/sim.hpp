#ifndef SIM_HPP
#define SIM_HPP	
class sim {
	public:
		sim (double * const, double * const, const double, const double);
		double getCurrentReward ();
		static const double TF_I, TF_II, TP_I, TP_II, ShorPen_Rate, discR;

	private:
		double currentReward;
		
		double * cashLevel, * const assetLevel;
		double cashInflow;

		const double cash_flow, cash_action;

		void simulation();
			double cost_by_trans;
			double cost_by_shortage;
};
#endif
