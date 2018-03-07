//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include "sim.cpp"
#include <float.h>

DP :: DP () : maxC_value(100), maxA_value(300), discrete(1.0), smallNum(0.0001), c_mean(-5.0), c_std(5.0), discR(1.0/1.02), action_step_size(1), returnRate(0.05){
	Reader.open("Policy.csv");
	maxC = floor(maxC_value/discrete+0.5);
	maxA = floor(maxA_value/discrete+0.5);
	OldV = new double * [maxC+1];
	NewV = new double * [maxC+1];
	Policy = new double * [maxC+1];
	for(int i = 0; i <= maxC; ++i){
		OldV[i] = new double [maxA+1];
		NewV[i] = new double [maxA+1];
		Policy[i] = new double [maxA+1];
		
		for(int j = 0; j <= maxA; ++j){
			OldV [i][j] = 0.0;
			NewV[i][j] = 0.0;
			Reader >> Policy [i][j] >> t;
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
			temp_Qvalue = Qvalue(i, j, Policy[i][j]);
			NewV[i][j] = temp_Qvalue;;
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
/*
int DP :: FindAllAction(int x, int y, double* min, double* max){
	count_cur = 1;
	cash_cur = State_Val_cash(x);
	asset_cur = State_Val_asset(y);
	asset_cur += returnRate * asset_cur;
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
*/
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
		asset_Q += returnRate * asset_Q;
		cash_net = cash_flow[it];
		sim obj(&cash_Q, &asset_Q, cash_net, ACT);
		if(cash_Q <=0 && asset_Q <=0){
			current = 1.0;
			future = 0.0;
		}
		else{
			current = 0.0;
			future = interpolation(cash_Q, asset_Q);	
		}
		sum += (current + future) * cash_prob[it];
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
	value_OP.open("Value.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			value_OP << NewV[i][j]<<',';
		}
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
		delete [] OldV[i];
		delete [] Policy[i];
		delete [] NewV[i];
	}
	delete [] OldV;
	delete [] NewV;
	delete [] Policy;
}
