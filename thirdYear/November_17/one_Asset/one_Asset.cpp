//one_Asset.cpp

#include <iostream>
#include "one_Asset.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;
const double one_Asset :: ReturnRate = 0.05;
const double one_Asset :: TF_I = 1.0;
const double one_Asset :: TF_II = 2.0;
const double one_Asset :: TP_I = 0.1;
const double one_Asset :: TP_II = 0.2;
const double one_Asset :: SCR = 2.0;
const double one_Asset :: discR = 1.0/1.02;
const double one_Asset :: smallNum = 0.0001;
const double one_Asset :: c_mean = 0.0;
const double one_Asset :: c_std = 2.5;
const double one_Asset :: Action_size = 1.0;
const int one_Asset :: MaxC = 100;
one_Asset :: one_Asset () {

	OldV = new double [MaxC+1];
	NewV = new double [MaxC+1];
	OptAction_index = new int [MaxC+1];
	totalNumber_of_Actions = new int [MaxC+1];
	PolicyTable = new double * [MaxC+1];
	cout << "Here is fine."<<endl;//8***************
	for( int i = 0; i <= MaxC; ++i){
		OldV[i] = 0.0;
		NewV[i] = 0.0;
		OptAction_index[i] = 0;
		//************************
		double min, max;
		min = (State_to_Value(i)-TF_I)/(1+TP_I);
		if(min < 0)
			min = 0;
		max = State_to_Value(MaxC) - State_to_Value(i);
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
	return state*1.0;
}
int one_Asset :: Value_to_State(double value){
	int s = floor (value+0.5);
	if(s > MaxC)
		s = MaxC;
	else if (s < 0)
		s = 0;
	else;
	return s;
}
//double one_Asset :: simulation (double * cash_s, double demand_s, int Action_ind){
//}



