//one_Asset.cpp
#include <iostream>
#include "one_Asset.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;
const double one_Asset :: ReturnRate = 0.05;
const double one_Asset :: TF_I = 1.5;
const double one_Asset :: TF_II = 3;
const double one_Asset :: TP_I = 0.15;
const double one_Asset :: TP_II = 0.3;
const double one_Asset :: SCR = 2.0;
const double one_Asset :: discR = 1.0/1.02;
const double one_Asset :: smallNum = 0.0001;
const double one_Asset :: c_std = 5;
const double one_Asset :: Action_size = 0.1;
const int one_Asset :: MaxC = 200;
one_Asset :: one_Asset (double c):c_mean(c) {
	OldV = new double [MaxC+1];
	NewV = new double [MaxC+1];
	OptAction_index = new int [MaxC+1];
	totalNumber_of_Actions = new int [MaxC+1];
	PolicyTable = new double * [MaxC+1];
	for( int i = 0; i <= MaxC; ++i){
		OldV[i] = 0.0;
		NewV[i] = 0.0;
		OptAction_index[i] = 0;
		double min, max;
		min = (State_to_Value(i)-TF_I)/(1+TP_I);
		if(min < 0)
			min = 0;
		max = (State_to_Value(MaxC) - State_to_Value(i)+TF_II)/(1-TP_II);
		if(max < 0)
			max = 0;
		int ActionNumber = 1;
		for(double action = -min; action <= max; action += Action_size){
			ActionNumber++;
		}
		PolicyTable[i] = new double [ActionNumber];
		totalNumber_of_Actions[i] = ActionNumber;
		PolicyTable[i][0] = 0;
		int a = 1;
		for(double action = -min; action <= max; action+= Action_size){
			PolicyTable[i][a] = action;
			a++;
		}
	}
	ReadHermite();
	Repetition();	
}
one_Asset :: ~one_Asset () {
	for (int i = 0; i <= MaxC ; ++i)
		delete [] PolicyTable[i];
	delete [] PolicyTable;
	delete [] OldV;
	delete [] NewV;
	delete [] OptAction_index;
}
void one_Asset :: ReadHermite () {
	Hermite reader(c_mean, c_std);
	for(vector<double> :: size_type it = 0, end = reader.demand.size(); it < end; ++it){
		cash_demand.push_back(reader.demand[it]);
		cash_prob.push_back(reader.probability[it]);
	}
}
double one_Asset :: State_to_Value(int state){
	return state*0.2;
}
int one_Asset :: Value_to_State(double value){
	int s = floor (value*5.0+0.5);
	if(s > MaxC)
		s = MaxC;
	else if (s < 0)
		s = 0;
	else;
	return s;
}
double one_Asset :: simulation (double * cash_s, double cash_net, double Action_value){
	cost_by_transaction = TranCost(Action_value);
	*cash_s = *cash_s + Action_value - cost_by_transaction;
	cost_by_holding = ReturnRate * *cash_s;
	*cash_s += cash_net;
	if(*cash_s >= 0)
		cost_by_shortage = 0;
	else{
		cost_by_shortage = SCR * abs(*cash_s);
		*cash_s = 0.0;
	}
	return cash_net - cost_by_transaction - cost_by_shortage - cost_by_holding;
	
}
double one_Asset :: TranCost(double Action_value){
	if(Action_value > 0)
		return TF_II + TP_II * Action_value;
	else if (Action_value < 0)
		return TF_I + TP_I * abs(Action_value);
	else return 0;

}
void one_Asset :: CalForOneStage(){
	for(int i = 0; i <= MaxC; ++i){
		best_value_s = - 9999.9;
		best_policy_index = 0;
		for(int policy_index = 0; policy_index < totalNumber_of_Actions[i]; ++policy_index){
			double temp = Qvalue(i, PolicyTable[i][policy_index]);
			if(best_value_s < temp){
				best_value_s = temp;
				best_policy_index = policy_index;
			}
		}
		NewV[i] = best_value_s;
		OptAction_index[i] = best_policy_index;
	}
}
double one_Asset :: Qvalue (int s, double Action){
	double cash_cal, current, future;
	double sum = 0;
	for (vector<double>::size_type it = 0, end = cash_demand.size(); it < end; ++it){
		cash_cal = State_to_Value(s);
		current = simulation(&cash_cal, cash_demand[it], Action);
		future = interpolation(cash_cal);
		sum += (current + discR * future) *cash_prob[it];
	}
	return sum;
}
double one_Asset :: interpolation(double x){
	if (x*5 > MaxC)
		x = MaxC*0.2;
	else if (x < 0)
		x = 0;
	xf = floor (x*5);
	xc = ceil (x*5);
		return (x*5 - xf) * (OldV[xc] - OldV[xf]) + OldV[xf];
}
bool one_Asset :: Comparison () {
	maxdifference = 0.0;
	for (int i = 0; i <= MaxC; ++i){
		difference = abs(NewV[i] - OldV[i]);
		if(maxdifference < difference)
			maxdifference = difference;
	}
//	cout << "We are at stage: "<<stagecounter<<endl;
//	cout << "The max difference is: "<<maxdifference<<endl;
	if(maxdifference <= smallNum)
		return 1;
	else return 0;
}
void one_Asset :: Repetition () {
	stagecounter = 1;

	STP = 0;
	while(STP != 1){
		cout << "We are at stage: "<<stagecounter<<endl;
		CalForOneStage();
		STP = Comparison();
		update();
		stagecounter++;
	}
}
void one_Asset :: update() {
	for (int i = 0; i <= MaxC; ++i){
		OldV [i] = NewV[i];
	}
}

