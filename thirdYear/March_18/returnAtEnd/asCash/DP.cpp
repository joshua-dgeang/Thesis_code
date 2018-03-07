//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include "sim.cpp"
#include <float.h>

DP :: DP () : maxC_value(300), maxA_value(300), discrete(1.0), smallNum(0.0001), c_mean(-5.0), c_std(5.0), discR(1.0/1.02), action_step_size(1), returnRate(0.05){
	maxC = maxC_value/discrete;
	maxA = maxA_value/discrete;
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
			counter_ini = FindAllAction(i,j,&min_ini, &max_ini);//This function will return the total number of possible actions given the state i,j. Meanwhile it will set mini_ini to the max amount of money could be transfered from cash to asset and set max_ini to the max amount of money could be transfered from asset to cash;
			PolicyTable[i][j] = new double [counter_ini];
			NumPolicy[i][j] = counter_ini;
			PolicyTable[i][j][0] = 0;
			min_ini = -min_ini;
			for(int h = 1; h < counter_ini; ++h){
				if(min_ini >= smallNum || min_ini <= -smallNum)
					PolicyTable[i][j][h] = min_ini;
					min_ini += action_step_size;
			}
		}
	}
	//********************************** END OF INITIALIZATION **************************************
	HermiteReader();
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
	cash_cur += returnRate * asset_cur;
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
		if(m >= smallNum || m <= -smallNum)
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
	if(x > State_Val_cash(maxC)) x = State_Val_cash(maxC);
	if(y > State_Val_asset(maxA)) y = State_Val_asset(maxA);//x and y are the cash/asset values. Here to make sure that they don't exceed the maximum value of this model.
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	scf=floor(x/discrete);
	saf=floor(y/discrete);
	scc=ceil(x/discrete);
	sac=ceil(y/discrete);

	VA = OldV[scf][saf];
	VB = OldV[scc][saf];
	VC = OldV[scc][sac];
	VD = OldV[scf][sac];
	VF = (x/discrete-scf) * (VB - VA) + VA;
	VE = (x/discrete-scf) * (VC - VD) + VD;
	VG = (y/discrete-saf) * (VE -VF) + VF;
	return VG;
}

double DP :: Qvalue(int x, int y, double ACT){
	sum = 0;
	for(vector<double>::size_type it = 0, end = cash_flow.size(); it < end; ++it){
		cash_Q = State_Val_cash(x);
		asset_Q = State_Val_asset(y);
		cash_net = cash_flow[it];
        cash_Q += returnRate * asset_Q;
		sim obj(&cash_Q, &asset_Q, cash_net, ACT);
        //cash_Q += returnRate * asset_Q;
        /*if(cash_Q > maxC_value)
            cash_Q = maxC_value;*/
		current = obj.getCurrentReward();
		future = interpolation(cash_Q, asset_Q);	
		sum += (current + discR * future) * cash_prob[it];
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
	return c * discrete;
}
double DP :: State_Val_asset(int a){
	return a * discrete;
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
	int temp = floor(c/discrete+0.5);
	if(temp > maxC)
		return maxC;
	else if (temp < 0)
		return 0;
	return temp;
}
int DP :: Val_State_asset(double a){
	int temp = floor(a/discrete+0.5);
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
