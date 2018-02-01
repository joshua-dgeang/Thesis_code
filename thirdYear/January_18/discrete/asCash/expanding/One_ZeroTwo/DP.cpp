//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include "sim.cpp"
#include <float.h>

DP :: DP () : maxC_value(100), maxA_value(300), discrete_reader(1.0), discrete(0.2), smallNum(0.0001), c_mean(0.0), c_std(5.0), discR(1.0/1.02), action_step_size(0.2), returnRate(0.05){
	maxC_reader = floor(maxC_value / discrete_reader+0.5);
	maxA_reader = floor(maxA_value / discrete_reader+0.5);
	maxC = floor(maxC_value/discrete+0.5);
	maxA = floor(maxA_value/discrete+0.5);
	ReaderValue = new double * [maxC_reader+1];
	OldV = new double * [maxC+1];
	NewV = new double * [maxC+1];
	OptPolicy_index = new int * [maxC+1];
	NumPolicy = new int * [maxC+1];
	PolicyTable = new double ** [maxC+1];

	Reader.open("Reader.csv");
	if(!Reader.is_open()){
		cerr  << "Couldn't open the Reader.csv file!"<<endl;
		exit(EXIT_FAILURE);
	}
	else;
	for (int i = 0; i <= maxC_reader; ++i){
		ReaderValue[i] = new double [maxA_reader+1];
		for(int j = 0; j <= maxA_reader; ++j){
			Reader >> ReaderValue[i][j] >> t;
		}
	}
	//***************** Test the reading function ***********************
	/*
	value_OP.open("testfile.csv");
	for(int i = 0; i <= maxC_reader; ++i){
		for(int j = 0; j <= maxA_reader;++j){
			value_OP << ReaderValue[i][j] << ',';
		}	
		value_OP << endl;
	}
	exit(0);
	*/
	//***************** Test the reading function ***********************

	for(int i = 0; i <= maxC; ++i){
		OldV[i] = new double [maxA+1];
		NewV[i] = new double [maxA+1];
		OptPolicy_index[i] = new int [maxA+1];
		NumPolicy[i] = new int [maxA+1];
		PolicyTable[i] = new double * [maxA+1];
		
		for(int j = 0; j <= maxA; ++j){
			double cash_temp, asset_temp;
			int i_temp, j_temp;
			//*********** Initialise OldV *************
			cash_temp = State_Val(i, discrete);
			asset_temp = State_Val(j, discrete);
			i_temp = Val_State(cash_temp, maxC_reader, discrete_reader);
			j_temp = Val_State(asset_temp, maxA_reader, discrete_reader);
			OldV[i][j] = ReaderValue[i_temp][j_temp];
			//*********** Initialise OldV *************
			NewV[i][j] = OldV[i][j];
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
	cash_cur = State_Val(x,discrete);
	asset_cur = State_Val(y,discrete);
	cash_cur += returnRate * asset_cur;
	A_cur = State_Val(maxC, discrete) - cash_cur;
	B_cur = (asset_cur - sim::TF_II)/(1+sim::TP_II);
	C_cur = State_Val(maxA, discrete) - asset_cur;
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
	if(x > State_Val(maxC, discrete)) x = State_Val(maxC, discrete);
	if(y > State_Val(maxA,discrete)) y = State_Val(maxA, discrete);//x and y are the cash/asset values. Here to make sure that they don't exceed the maximum value of this model.
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
		cash_Q = State_Val(x,discrete);
		asset_Q = State_Val(y,discrete);
		cash_net = (returnRate*asset_Q- 5.0) + cash_flow[it];
		sim obj(&cash_Q, &asset_Q, cash_net, ACT);
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

double DP :: State_Val(int a, double dis){
	return a * dis;
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
int DP :: Val_State(double c, int MAX, double dis){
	int temp = floor(c/dis+0.5);
	if(temp > MAX)
		return MAX;
	else if (temp < 0)
		return 0;
	return temp;
}

DP :: ~ DP () {
	for(int i = 0; i <= maxC_reader; ++i){
		delete [] ReaderValue[i];
	}
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
	delete [] ReaderValue;
	delete [] OldV;
	delete [] NewV;
	delete [] NumPolicy;
	delete [] OptPolicy_index;
	delete [] PolicyTable;
}
