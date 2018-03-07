//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include "sim.cpp"
#include <float.h>
#include <cmath>

DP :: DP ():maxC_value(100),maxA_value(300),discrete(1.0),smallNum(0.0001),c_mean(-5.0),c_std(5.0),discR(1.0/1.02),action_step_size(1.0),returnRate(0.05),loanSize(40),loanRate(0.03),maxLoanTime(40){
	//*********** Initialisation I *******************
	loanPayment = loanSize * (loanRate * pow(1+loanRate,maxLoanTime)) / (pow(1+loanRate,maxLoanTime)-1);
	maxC = floor(maxC_value / discrete);
	maxA = floor(maxA_value / discrete);
	//*********** Initialisation II *******************
	LoanAction = new bool * [maxC+1];
	for(int i = 0; i <= maxC; ++i){
		LoanAction[i] = new bool [maxA+1];
	}
	OldV = new double ** [maxLoanTime+1];
	NewV = new double ** [maxLoanTime+1];
	OptPolicy_index = new int ** [maxLoanTime+1];
	NumPolicy = new int ** [maxLoanTime+1];
	PolicyTable = new double *** [maxLoanTime+1];
	for(int l = 0; l <= maxLoanTime; ++l){
		OldV[l] = new double * [maxC +1];
		NewV[l] = new double * [maxC +1];
		OptPolicy_index[l] = new int * [maxC+1];
		NumPolicy[l] = new int * [maxC + 1];
		PolicyTable[l] = new double ** [maxC+1];
		for(int i = 0; i <= maxC; ++i){
			OldV[l][i] = new double [maxA +1];
 			NewV[l][i] = new double [maxA +1];
 			OptPolicy_index[l][i] = new int [maxA+1];
 			NumPolicy[l][i] = new int [maxA + 1];
 			PolicyTable[l][i] = new double * [maxA+1];
			for(int j = 0; j <= maxA; ++j){
				OldV[l][i][j] = 0.0;
				NewV[l][i][j] = 0.0;
				OptPolicy_index[l][i][j] = 5005;
				NumPolicy[l][i][j] = 5005;
				//find the total number of feasible actions given a state (l,i,j)
					min_ini = 0;
					max_ini = 0;
					//...
					counter_ini = FindAllAction(l,i,j,&min_ini, &max_ini);
					PolicyTable[l][i][j] = new double [counter_ini];
					NumPolicy[l][i][j] = counter_ini;
					PolicyTable[l][i][j][0] = 0;
					min_ini = - min_ini;
					for(int h = 1; h < counter_ini; ++h){
						if(min_ini >= smallNum || min_ini <= -smallNum){
							PolicyTable[l][i][j][h] = min_ini;
							min_ini+=action_step_size;
						}
					}
			}
		}
	}
//End of initialisation...
	
}






 double DP :: State_Val(int state){
	
         return state * discrete;        
 }                                   
int DP :: Val_State_cash(double value, int maxState){
	tempState = floor(value/discrete + 0.5);
	if(maxState == 0)
		return tempState;
	else if (tempState > maxState)
		return maxState;
	else
		return tempState;
}



int DP :: FindAllAction(int z, int x, int y, double* min, double* max){
        count_cur = 1;
        cash_cur = State_Val(x);
        asset_cur = State_Val(y);
        cash_cur += returnRate * asset_cur;
	if(z == 0)
		cash_cur+=loanSize;
	else
		cash_cur-=loanPayment;
        A_cur = State_Val(maxC) - cash_cur;
        B_cur = (asset_cur - sim::TF_II)/(1+sim::TP_II);
        C_cur = State_Val(maxA) - asset_cur;
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
				
