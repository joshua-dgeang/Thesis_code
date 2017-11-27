//simu.hpp
//this class is used to simulate cash problem for one stage;
//users should pass two int pointers to the current state and two parameters (cash demand, cash action);
//this class will return the currentReward in this stage as well as the cash value and the asset value at the end this stage.
//this class will also modify the original state to the state in next stage.
#ifndef SIMU_HPP
#define SIMU_HPP
#include "../mainCode/TwoDiMod.hpp"
class simu {
	public:
		simu(int * const, int * const, bool * const, const double, const int);
		//~simu();
		double getCurrentRwd(), getCash_simu(), getAsest_simu();
		bool getSTP();
	private:
		static const int MaxC, MaxA;
		static const double ReturnRate, TF_I, TF_II, TP_I, TP_II, ShorPen, discR;
		double currentReward,  cash_simu, asset_simu;
		int * const S_c, * const S_a;
		bool * const stp;

		friend class twodimod;
		const double cash_demand;
		const int cash_transfer;
		
		void simulation ();
			double cost_by_trans;
			double cost_by_shortage;
};
#endif
