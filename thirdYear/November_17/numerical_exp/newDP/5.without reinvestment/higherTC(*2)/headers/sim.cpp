//sim.cpp
//this class is used to simulate cash problem for one stage;
//users shall pass two double pointers to the current cash and asset level and two constant parameters (cash demand, cash action);
//this class will return the immidiate reward for this stage as well as modify the two pointers (cash and asset)

#include "sim.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
const double sim :: ReturnRate = 0.05;
const double sim :: TF_I = 2.0;
const double sim :: TF_II = 4.0;
const double sim :: TP_I = 0.2;
const double sim :: TP_II = 0.4;
const double sim :: ShorPen = 10.0;

sim :: sim (double * const c, double * const as, double d, double ac):
	cashLevel(c), assetLevel(as), cash_demand(d), cash_action(ac){
		simulation();
	}

double sim :: getCurrentReward() {
	return currentReward;
}

void sim :: simulation () {
	cashInflow = ReturnRate * *assetLevel;
	*cashLevel += cashInflow;
	//cout << "After cash inflow: "<<*cashLevel<<':'<<*assetLevel<<endl;//_______________________
	if(cash_action < 0){
		cost_by_trans = TF_I + TP_I * abs(cash_action);
		*cashLevel = *cashLevel - abs(cash_action) - cost_by_trans;
		//*assetLevel += abs(cash_action);
	}
	else if (cash_action > 0) {
		cost_by_trans = TF_II + TP_II * abs(cash_action);
		*cashLevel += abs(cash_action);
		//*assetLevel = *assetLevel - abs(cash_action) - cost_by_trans;
	}
	else
		cost_by_trans = 0.0;
	//cout << "Cost_by_trans: "<<cost_by_trans<<endl;//___________________________
	//cout <<"After transaction: "<<*cashLevel<<':'<<*assetLevel<<endl;//_________________________
    cost_by_cash = ReturnRate * *cashLevel;
	// *********************** cash demand occurs ******************
	*cashLevel -= cash_demand;
	//cout << "After demand: "<<*cashLevel<<':'<<*assetLevel<<endl;//________________________
	if(*cashLevel < 0){
		cost_by_shortage = ShorPen + TF_II + TP_II * abs(*cashLevel);
		//*assetLevel = *assetLevel - cost_by_shortage - abs(*cashLevel);
		*cashLevel = 0;
	}
	else
		cost_by_shortage = 0.0;
	//cout << "After adjust: "<<*cashLevel<<':'<<*assetLevel<<endl;//________________________
	//
	//************* CurrentReward depends on the objective function **************
	//
	currentReward = cashInflow - cost_by_trans - cash_demand - cost_by_shortage - cost_by_cash;
}


