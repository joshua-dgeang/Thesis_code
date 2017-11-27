//TwoDiMod.cpp
#include "TwoDiMod.hpp"
#include "../headers/Hermite.cpp"
#include "../headers/simu.cpp"
#include <cmath>
#include <vector>
twodimod :: twodimod ():MaxC(simu::MaxC),MaxA(simu::MaxA),smallNum(.0001),c_mean(5.0),c_std(2.5),dicF(1.0/1.02) {
	OldValue = new double * [MaxC+1];
	NewValue = new double * [MaxC+1];
	OptPolicy = new int * [MaxC+1];
	Bankruptcy = new bool * [MaxC+1];

	NumOfPolicy = new int * [MaxC+1];
	PolicyTable = new int ** [MaxC+1];
	
	for(int i = 0; i <= MaxC; ++i){
		OldValue[i] = new double [MaxA+1];
		NewValue[i] = new double [MaxA+1];
		OptPolicy[i] = new int [MaxA+1];
		Bankruptcy[i] = new bool [MaxA+1];

		NumOfPolicy[i] = new int [MaxA+1];
		PolicyTable[i] = new int * [MaxA+1];

		for(int j = 0; j <= MaxA; ++j){
			OldValue[i][j] = 0.0;
			NewValue[i][j] = 0.0;
			OptPolicy[i][j] = -5005;
			Bankruptcy[i][j] = 0;
			//***** FIND ALL POSSIBLE ACTIONS FOR EACH STATE *****
			asset_ini = j*1.0;
			cash_ini = i*1.0 + simu::ReturnRate * asset_ini;
			A_ini = MaxC*1.0 - cash_ini;
			B_ini = (asset_ini - simu::TF_II) / (1+simu::TP_II);
			C_ini = MaxA * 1.0 - asset_ini;
			D_ini = (cash_ini - simu::TF_I)/(1+simu::TP_I);
			if(A_ini <= B_ini)
					start_ini = floor(A_ini);                   
			else start_ini = floor(B_ini);                  
			if(start_ini < 0)
					start_ini = 0;                       
			else;                                    
			                                         
			if(C_ini <= D_ini)
					end_ini = floor(C_ini);                     
			else end_ini = floor(D_ini);
			if(end_ini < 0) end_ini= 0;              
			else;                             
			                                         
			counter_ini = 2;                         
			for(int s = start_ini; s >= -end_ini; s-=1){
				if(s!=0) counter_ini ++;     
			}                                        
			NumOfPolicy[i][j] = counter_ini;
			PolicyTable[i][j] = new int [counter_ini];
			PolicyTable[i][j][0] = 0;
			PolicyTable[i][j][1] = 5005;
			int temp_ini = 2;
			for(int a = start_ini; a >= -end_ini; a -= 1){
				if(a!=0){
					PolicyTable[i][j][temp_ini] = a;
					temp_ini ++;
				}
			}
		}
	}
	//******************************************
	HermiteReader();
	Repetition();
}
twodimod :: ~twodimod(){
	for(int i = 0; i <= MaxC; ++i){
		for(int j = 0; j <= MaxA; ++j){
			delete [] PolicyTable [i][j];
		}
		delete [] OldValue[i];
		delete [] NewValue[i];
		delete [] OptPolicy[i];
		delete [] Bankruptcy[i];
		delete [] NumOfPolicy[i];
	}
	delete [] OldValue;
	delete [] NewValue;
	delete [] OptPolicy;
	delete [] Bankruptcy;
	delete [] NumOfPolicy;
}

void twodimod :: HermiteReader(){
	Hermite cashdemandreader(c_mean, c_std);
	for(vector<double>::size_type it = 0, end = cashdemandreader.demand.size(); it < end; ++it){
		cash_demand.push_back(cashdemandreader.demand[it]);
		cash_prob.push_back(cashdemandreader.probability[it]);
	}
}
void twodimod :: CalForOneStage () {
	for(int i = 0; i <= MaxC; ++i)
		for(int j = 0; j <= MaxA; ++j){
			Bankruptcy[i][j] = 0;
			best_value_s = -10.0;
			best_policy_s = -5005;
			for(int action = 0; action < NumOfPolicy[i][j]; ++ action){
				double temp = Qvalue(i, j, PolicyTable[i][j][action]);
				if(best_value_s < temp){
					best_value_s = temp;
					best_policy_s = PolicyTable[i][j][action];
				}
			}
			if(best_value_s <= 0){
				best_value_s = 0;
				best_policy_s = 5005;
				Bankruptcy[i][j] = 1;
			}
			NewValue[i][j] = best_value_s;
			OptPolicy[i][j] = best_policy_s;
		}
}
double twodimod :: Qvalue(int x, int y, int a){
	bool stop = 0;
	int X, Y;
	double current, future;
	double sum = 0;
	for(vector<double>::size_type it = 0, end = cash_demand.size(); it < end; ++it){
		X = x;
		Y = y;
		stop = 0;
		simu obj(&X, &Y,&stop,cash_demand[it],a);
		current = obj.getCurrentRwd();
		future = interplotation(obj.getCash_simu(),obj.getAsest_simu());
		sum += (current + dicF*future) * cash_prob[it];
	}	
	//*****************************
	if(x == 10 && y == 100 && a == 0)
		cout << sum << endl;
	return sum;	
}
double twodimod :: interplotation(double x, double y){
	if(x > MaxC) x = MaxC * 1.0;
	if(y > MaxA) y = MaxA * 1.0;
	if(y < 0) y = 0;
	xf = floor(x);
	yf = floor(y);
	xc = ceil(x);
	yc = ceil(y);
	VA = OldValue[xf][yf];
	VB = OldValue[xc][yf];
	VC = OldValue[xc][yc];
	VD = OldValue[xf][yc];
	VF = (x-xf)*(VB - VA) + VA;
	VE = (x-xf)*(VC - VD) + VD;
	VG = (y-yf)*(VE - VF) + VF;
	return VG;
}

bool twodimod :: Comparison(){
	maxdifference = 0.0;
	for(int i = 0; i <= MaxC; ++i){
		for(int j = 0; j <= MaxA; ++j){
			difference = abs(NewValue[i][j] - OldValue[i][j]);
			if(maxdifference < difference){
			maxdifference = difference;
			//cout << i <<'/'<<j<<endl;	
			
			}
		}
	}
	cout << "We are at stage: "<<stagecounter<<endl;
	cout << "The maxdifference is: "<<maxdifference<<endl;
	
	if(maxdifference <= smallNum)
		return 1;
	else return 0;
}
void twodimod :: Repetition () {
	stagecounter = 1;
	STP = 0;
	while(STP != 1){
		CalForOneStage();
		STP = Comparison();
		update();
		stagecounter++;
	}
}
void twodimod :: update(){
	for(int i = 0; i <= MaxC; ++i){
		for(int j = 0; j<= MaxA; ++j){
			OldValue[i][j] = NewValue[i][j];
		}
	}
}
