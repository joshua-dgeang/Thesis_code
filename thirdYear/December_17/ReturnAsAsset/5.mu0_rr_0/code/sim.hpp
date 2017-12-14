#ifndef SIM_HPP
#define SIM_HPP	
class sim {
	public:
		sim (double * const, double * const, const double, const double);
		double getCurrentReward ();
		static const double TF_I, TF_II, TP_I, TP_II, ShorPen_Rate, Return_Rate;

	private:
		double currentReward;
		
		double * cashLevel, * const assetLevel;
		double proft;

		const double cash_flow, cash_action;

		void simulation();
			double cost_by_trans;
			double cost_by_shortage;
};
#endif
