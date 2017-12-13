//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include <float.h>
#include "sim.cpp"

DP :: DP () : maxC(100), maxA(300), smallNum(0.0001), c_mean(-5.0), c_std(2.5), discR(1.0/1.02), action_step_size(1.0){
	OldV = new double * [maxC+1];
	NewV = new double * [maxC+1];
	OptPolicy_index = new int * [maxC+1];
	NumPolicy = new int * [maxC+1];
	PolicyTable = new double ** [maxC+1];
	for(int i = 0; i <= maxC; ++i){
		OldV[i] = new double [maxA+1];
		NewV[i] = new double [maxA+1];
		OptPolicy_index[i] = new int [maxA+1];
		NumPolicy[i] = new int [maxA+1];
		PolicyTable[i] = new double * [maxA+1];
		
		for(int j = 0; j <= maxA; ++j){
			OldV [i][j] = 0.0;
			NewV[i][j] = 0.0;
			OptPolicy_index[i][j] = 5005;
			NumPolicy[i][j] = 5005;
			min_ini = 0;
			max_ini = 0;
			counter_ini = FindAllAction(i,j,&min_ini, &max_ini);
			PolicyTable[i][j] = new double [counter_ini];
			NumPolicy[i][j] = counter_ini;
			PolicyTable[i][j][0] = 0;
			min_ini = -min_ini;
			for(int h = 1; h < counter_ini; ++h){
				if(min_ini != 0)
					PolicyTable[i][j][h] = min_ini;
					min_ini += action_step_size;
			}
		}
	}
	//********************************** END OF INITIALIZATION **************************************
	HermiteReader();
	Repitition();
	Repitition();
	TestOutPut();
	
}
void DP :: CalForOneStage () {
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			best_value_s = -DBL_MAX;
			best_policy_index = 0;
	//cout << "Here is fine"<<i<<"+"<<j<<endl;
			for(int policy_indx = 0; policy_indx < NumPolicy[i][j]; ++ policy_indx){

				
				
				temp_Qvalue = Qvalue(i, j, PolicyTable[i][j][policy_indx]);
				
				if(best_value_s < temp_Qvalue){
					best_value_s = temp_Qvalue;
					best_policy_index = policy_indx;
				}
			}

			NewV[i][j] = best_value_s;
			OptPolicy_index[i][j] = best_policy_index;
		}
}
bool DP :: Comparison () {
	maxdifference = 0.0;
	for (int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			difference = abs (NewV[i][j] - OldV[i][j]);
			if (maxdifference < difference){
				maxdifference = difference; 
			}
		}

	cout << "We are at stage: "<<stage_counter << endl; 
	cout << "The maxdifference is: "<<maxdifference << endl;
	if(maxdifference <= smallNum)
		return 1;
	else 
		return 0;

}
int DP :: FindAllAction(int x, int y, double* min, double* max){
	count_cur = 1;
	cash_cur = State_Val_cash(x);
	asset_cur = State_Val_asset(y);
	A_cur = State_Val_cash(maxC) - cash_cur;
	B_cur = (asset_cur - sim::TF_II)/(1+sim::TP_II);
	C_cur = State_Val_asset(maxA) - asset_cur;
	D_cur = (cash_cur - sim::TF_I)/(1+sim::TP_I);
	if(A_cur <= B_cur)
		*max = floor(A_cur);
	else 
		*max = floor (B_cur);
	if(*max <0) 
		*max = 0;
	else;
	if(C_cur <= D_cur)
		*min = floor(C_cur);
	else 
		*min = floor(D_cur);
	if(*min <0)
		*min = 0;
	else;

	for(double m = -*min; m<= *max; m+=action_step_size){
		if(m != 0)
			count_cur++;
	}
	return count_cur;
}
void DP :: HermiteReader() {
	Hermite reader (c_mean, c_std);
	for(vector<double> :: size_type it = 0, end = reader.demand.size(); it < end; ++it){
		cash_flow.push_back(reader.demand[it]);
		cash_prob.push_back(reader.probability[it]);
	}
}
double DP :: interpolation (double x, double y) {
	if(x > maxC) x = maxC * 1.0;
	if(y > maxA) y = maxA * 1.0;
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	xf = floor(x);
	yf = floor(y);
	xc = ceil(x);
	yc = ceil(y);
	VA = OldV[xf][yf];
	VB = OldV[xc][yf];
	VC = OldV[xc][yc];
	VD = OldV[xf][yc];
	VF = (x-xf) * (VB - VA) + VA;
	VE = (x-xf) * (VC - VD) + VD;
	VG = (y-yf) * (VE -VF) + VF;
	return VG;
}

double DP :: Qvalue(int x, int y, double ACT){
		if(x == 90 && y == 100 && ACT == 0){
			cout << "*********************************"<<endl;
		}
	
	sum = 0;
	for(vector<double>::size_type it = 0, end = cash_flow.size(); it < end; ++it){
		cash_Q = State_Val_cash(x);
		asset_Q = State_Val_asset(y);
		cash_net = cash_flow[it];
		sim obj(&cash_Q, &asset_Q, cash_net, ACT);
		current = obj.getCurrentReward();
		future = interpolation(cash_Q, asset_Q);	
		sum += (current + discR * future) * cash_prob[it];
		/*
		
		if(x == 90 && y == 100 && ACT == 0){
			cout << "ACT=0: "<< cash_net<<"*"<<"\t\t\t\tSUM= "<<sum<<" = "<< current<<":"<<future << endl;
		}
		if(x == 90 && y == 100 && ACT == -50){
			cout <<"_________"<<endl;
			cout << "ACT=-50: "<< cash_net<<"*"<<"\t\t\t\tSUM= "<<sum<<" = "<< current<<":"<<future << endl;
		}
		*/


	}

	return sum;

}
void DP :: Repitition() {
	stage_counter = 1;
	STP = 0;
	while(STP != 1 ){
		CalForOneStage();
		STP = Comparison();
		update();
		stage_counter++;
	}
}
double DP :: State_Val_cash(int c){
	return c * 1.0;
}
double DP :: State_Val_asset(int a){
	return a * 1.0;
}
void DP :: TestOutPut(){
	policy_OP.open("Policy.csv");
	value_OP.open("Value.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			policy_OP << PolicyTable[i][j][OptPolicy_index[i][j]]<<',';
			value_OP << NewV[i][j]<<',';
		}
		policy_OP << endl;
		value_OP << endl;
	}
}
void DP :: update(){
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			OldV[i][j] = NewV[i][j];
		}
}
int DP :: Val_State_cash(double c){
	int temp = floor(c+0.5);
	if(temp > maxC)
		return maxC;
	else if (temp < 0)
		return 0;
	return temp;
}
int DP :: Val_State_asset(double a){
	int temp = floor(a+0.5);
	if(temp > maxA)
		return maxA;
	else if (temp < 0)
		return 0;
	return temp;
}
DP :: ~ DP () {
	for (int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			delete [] PolicyTable[i][j];
		}
		delete [] OldV[i];
		delete [] NewV[i];
		delete [] NumPolicy[i];
		delete [] OptPolicy_index[i];
		delete [] PolicyTable[i];
	}
	delete [] OldV;
	delete [] NewV;
	delete [] NumPolicy;
	delete [] OptPolicy_index;
	delete [] PolicyTable;
}
