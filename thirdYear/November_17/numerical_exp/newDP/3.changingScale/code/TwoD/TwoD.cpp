//TwoD.cpp
#include "TwoD.hpp"
#include <cstdlib>
#include <cmath>
#include "../headers/Hermite.cpp"
#include "../headers/sim.cpp"
using namespace std;

TwoD :: TwoD () : MaxC(200), MaxA(400), smallNum(0.0001), c_mean(5.0), c_std(2.5), discR(1.0/1.02), action_step_size(1.0){
	OldValue = new double * [MaxC+1];
	NewValue = new double * [MaxC+1];
	OptPolicy = new int * [MaxC+1];
	NumPolicy = new int * [MaxC+1];
	PolicyTable = new double ** [MaxC+1];

	for(int i = 0; i <= MaxC; ++i){
		OldValue[i] = new double [MaxA + 1];
		NewValue[i] = new double [MaxA + 1];
		OptPolicy[i] = new int [MaxA + 1];
		NumPolicy[i] = new int [MaxA + 1];
		PolicyTable[i] = new double * [MaxA+1];
		for(int j = 0; j <= MaxA; ++j){
			OldValue [i][j] = -999.0;
			NewValue [i][j] = -999.0;
			OptPolicy [i][j] = -5005;
			//*************** FIND ALL POSSIBLE ACTIONS FOR EACH STATE ***************
			cash_ini = StatetoVal_cash(i);
			asset_ini = StatetoVal_asset(j);
			cash_ini += sim :: ReturnRate * asset_ini;
			A_ini = StatetoVal_cash(MaxC) - cash_ini;
			B_ini = (asset_ini - sim :: TF_II)/(1+sim::TP_II);
			C_ini = StatetoVal_asset(MaxA) - asset_ini;
			D_ini = (cash_ini - sim::TF_I)/(1+sim::TP_I);

			if( A_ini <= B_ini)
				end_ini = floor(A_ini);
			else end_ini = floor(B_ini);
			if(end_ini < 0)
				end_ini = 0;
			else;

			if(C_ini <= D_ini)
				start_ini = floor(C_ini);
			else start_ini = floor(D_ini);
			if(start_ini <0)
				start_ini = 0;
			else;
			counter_ini = 1;
			for(double s = -start_ini; s <= end_ini; s+=action_step_size){
					counter_ini++;
			}
			NumPolicy[i][j] = counter_ini;
			PolicyTable[i][j] = new double [counter_ini];
			PolicyTable[i][j][0] = 0.0;
			double ac_init = -start_ini;
			for(int tep = 1; tep < counter_ini; tep++){
					PolicyTable[i][j][tep] = ac_init;
					ac_init += action_step_size;
			}
		}
        
	}

	HermiteReader();
    
	Repetition();

}

TwoD :: ~ TwoD() {
	TestOutPut();
	for(int i = 0; i <= MaxC; ++i){
		for(int j = 0; j <= MaxA; ++j){
			delete [] PolicyTable[i][j];
		}
		delete [] OldValue [i];
		delete [] NewValue [i];
		delete [] OptPolicy[i];
		delete [] NumPolicy [i];
		delete [] PolicyTable[i];
	}
	delete [] PolicyTable;
	delete [] OldValue;
	delete [] NewValue;
	delete [] OptPolicy;
	delete [] NumPolicy;
}

void TwoD :: HermiteReader () {

	Hermite reader(c_mean, c_std);
	for(vector<double> :: size_type it = 0, end = reader.demand.size(); it < end; ++it){
		cash_demand.push_back(reader.demand[it]);
		cash_prob.push_back(reader.probability[it]);
	}
}

void TwoD :: CalForOneStage () {
	for(int i = 0; i <= MaxC; ++i)
		for(int j = 0; j <= MaxA; ++j){
            
			best_value_s = -999.0;
			best_policy_index = 0;
            
			for(int policyIndex = 0; policyIndex < NumPolicy[i][j]; ++ policyIndex){
              
                double temp = Qvalue(i, j, PolicyTable[i][j][policyIndex]);
             
				if(best_value_s < temp){
					best_value_s = temp;
					best_policy_index = policyIndex;
				}
			}
      
           
			NewValue[i][j] = best_value_s;
			OptPolicy[i][j] = best_policy_index;
            
           
            
		}
  
}
double TwoD :: Qvalue (int x, int y, double ACT){
 
    double cash_sim;
	double asset_sim;
	double current, future;
	double sum = 0;
    
	for(vector<double>::size_type it = 0, end = cash_demand.size(); it < end; ++it){
        
		cash_sim = StatetoVal_cash(x);
		asset_sim = StatetoVal_asset(y);
		sim obj(&cash_sim, &asset_sim, cash_demand[it],ACT);
		current = obj.getCurrentReward();
		future = interpolation (cash_sim, asset_sim);
		sum += (current + discR * future) * cash_prob[it];
	}
    
	return sum;
}
double TwoD :: interpolation(double x, double y){//different scale require a different interpolation function!!!

	if(x*2.0 > MaxC) x = MaxC * 0.5;
	if(y*2.0 > MaxA) y = MaxA * 0.5;
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	xf = floor(x*2.0);
	yf = floor(y*2.0);
	xc = ceil(x*2.0);
    yc = ceil(y*2.0);
	VA = OldValue[xf][yf];
	VB = OldValue[xc][yf];
	VC = OldValue[xc][yc];
	VD = OldValue[xf][yc];
	VF = (x*2.0-xf)*(VB - VA) + VA;
	VE = (x*2.0-xf)*(VC - VD) + VD;
	VG = (y*2.0-yf)*(VE - VF) + VF;
	return VG;
}

bool TwoD :: Comparison () {
	maxdifference = 0.0;
	for(int i = 0; i <= MaxC; ++i){
		for(int j = 0; j <= MaxA; ++j){
			difference = abs(NewValue[i][j] - OldValue[i][j]);
			if(maxdifference < difference){
				maxdifference = difference;
			}
		}
	}
	cout << "We are at stage: "<<stagecounter << endl;
	cout << "The maxdifference is: "<<maxdifference<<endl;

	if(maxdifference <= smallNum)
		return 1;
	else 
		return 0;
}

void TwoD :: Repetition () {
	stagecounter = 1;
	STP = 0; 
	while(STP != 1){
     
		CalForOneStage();
    
		STP = Comparison();
		update();
		stagecounter ++;
	}
}
void TwoD :: update () {
	for(int i = 0; i <= MaxC; ++i){
		for(int j = 0; j <= MaxA; ++j){
			OldValue [i][j] = NewValue[i][j];
		}
	}
}

int TwoD ::  ValtoState_cash(double c){
	int s_c = floor(c*2.0+0.5);
	if(s_c > MaxC)
		return MaxC;
	else if (s_c < 0)
		return 0;
	else
		return s_c;
}
int TwoD :: ValtoState_asset(double a){
	int s_a = floor(a*2.0+0.5);
	if(s_a > MaxA)
		return MaxA;
	else if (s_a < 0)
		return 0;
	else 
		return s_a;
}
double TwoD :: StatetoVal_cash (int c){
	return c*0.5;
}
double TwoD :: StatetoVal_asset(int a){
	return a*0.5;
}

void TwoD :: TestOutPut () {
			valueoutput.open("TwD_value.csv");
			policyoutput.open("TwD_policy.csv");
			if(!valueoutput.is_open()){
				cout << "error with value output."<<endl;
			}
			if(!policyoutput.is_open()){
				cout <<"error with policy output."<<endl;
			}
			else;
			for(int i = 0; i <= MaxC; ++i){
				for(int j = 0; j <= MaxA; ++j){
					valueoutput << NewValue[i][j]<<',';
					policyoutput << PolicyTable[i][j][OptPolicy[i][j]]<<',';
				}
				valueoutput << endl;
				policyoutput << endl;
			}
}
