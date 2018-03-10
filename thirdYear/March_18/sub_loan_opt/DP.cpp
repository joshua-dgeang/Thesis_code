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
	HermiteReader();
	Repitition (); 
	TestOutPut();
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

void DP :: HermiteReader() {                                                  
        Hermite reader (c_mean, c_std);                                       
        for(vector<double> :: size_type it = 0, end = reader.demand.size(); it < end; ++it){
                cash_flow.push_back(reader.demand[it]);                       
                cash_prob.push_back(reader.probability[it]);                  
        }                                                                     
}                                                                             

double DP :: interpolation (int loanstate, bool loanaction, double x, double y) {
	if(loanstate >= 1){
        	if(x > State_Val(maxC)) x = State_Val(maxC);
        	if(y > State_Val(maxA)) y = State_Val(maxA);
        	if(x < 0) x = 0;
        	if(y < 0) y = 0;
        	scf=floor(x/discrete);
        	saf=floor(y/discrete);
        	scc=ceil(x/discrete);
        	sac=ceil(y/discrete);

        	VA = OldV[loanstate-1][scf][saf];
        	VB = OldV[loanstate-1][scc][saf];
        	VC = OldV[loanstate-1][scc][sac];
        	VD = OldV[loanstate-1][scf][sac];
        	VF = (x/discrete-scf) * (VB - VA) + VA;
        	VE = (x/discrete-scf) * (VC - VD) + VD;
        	VG = (y/discrete-saf) * (VE -VF) + VF;
        	return VG;
	}
	else if (loanaction == 0){
		if (loanstate != 0){
			cerr << "Error in interpolation!!!"<<endl;
			exit(EXIT_FAILURE);
		}
        	if(x > State_Val(maxC)) x = State_Val(maxC);
        	if(y > State_Val(maxA)) y = State_Val(maxA);
        	if(x < 0) x = 0;
        	if(y < 0) y = 0;
        	scf=floor(x/discrete);
        	saf=floor(y/discrete);
        	scc=ceil(x/discrete);
        	sac=ceil(y/discrete);

        	VA = OldV[loanstate][scf][saf];
        	VB = OldV[loanstate][scc][saf];
        	VC = OldV[loanstate][scc][sac];
        	VD = OldV[loanstate][scf][sac];
        	VF = (x/discrete-scf) * (VB - VA) + VA;
        	VE = (x/discrete-scf) * (VC - VD) + VD;
        	VG = (y/discrete-saf) * (VE -VF) + VF;
        	return VG;
	}
	else {
		if (loanstate != 0 || loanaction != 1){
			cerr << "Error_II in interpolation!!!"<<endl;
			exit(EXIT_FAILURE);
		}
        	if(x > State_Val(maxC)) x = State_Val(maxC);
        	if(y > State_Val(maxA)) y = State_Val(maxA);
        	if(x < 0) x = 0;
        	if(y < 0) y = 0;
        	scf=floor(x/discrete);
        	saf=floor(y/discrete);
        	scc=ceil(x/discrete);
        	sac=ceil(y/discrete);
        	VA = OldV[maxLoanTime][scf][saf];
        	VB = OldV[maxLoanTime][scc][saf];
        	VC = OldV[maxLoanTime][scc][sac];
        	VD = OldV[maxLoanTime][scf][sac];
        	VF = (x/discrete-scf) * (VB - VA) + VA;
        	VE = (x/discrete-scf) * (VC - VD) + VD;
        	VG = (y/discrete-saf) * (VE -VF) + VF;
        	return VG;
	}
}                                                                
void DP :: Repitition() {          
        stage_counter = 1;         
        STP = 0;                   
        while(STP == 0 ){          
                CalForOneStage();  
                STP = Comparison();
                update();          
                stage_counter++;   
        }                          
}                                  

void DP :: update(){ 
	for(int l = 0; l <= maxLoanTime; ++l)
        	for(int i = 0; i <= maxC; ++i)          
                	for(int j = 0; j <= maxA; ++j){ 
                        	OldV[l][i][j] = NewV[l][i][j];
                }                               
}                                               

bool DP :: Comparison () {
        maxdifference = 0.0;
	for(int l = 0; l <= maxLoanTime; ++l)
        	for (int i = 0; i <= maxC; ++i)
                	for(int j = 0; j <= maxA; ++j){
                        	difference = abs (NewV[l][i][j] - OldV[l][i][j]);
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
void DP::CalForOneStage() {
	//Companies without any debt:
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			//Best value taking loan:
			best_value_takingloan = -DBL_MAX;
			best_policy_index_takingloan = 0;
			loan_option_calforone = 1;
			for(int policy_index = 0; policy_index < NumPolicy[0][i][j]; ++ policy_index){
				temp_Qvalue = Qvalue(0, i, j, PolicyTable[0][i][j][policy_index], loan_option_calforone);
				if(best_value_takingloan < temp_Qvalue){
					best_value_takingloan = temp_Qvalue;
					best_policy_index_takingloan = policy_index;
				}
			}
			//Best value NOT taking loan:
			best_value_s = -DBL_MAX;
			best_policy_index = 0;
			loan_option_calforone = 0;
			for(int policy_index = 0; policy_index < NumPolicy[0][i][j]; ++ policy_index){
				temp_Qvalue = Qvalue(0, i, j, PolicyTable[0][i][j][policy_index], loan_option_calforone);
				if(best_value_s < temp_Qvalue){
					best_value_s = temp_Qvalue;
					best_policy_index = policy_index;
				}
			}
			//Comparing taking loan and not taking loan, record the result:
			if(best_value_s >= best_value_takingloan){
				LoanAction[i][j] = 0;
				NewV[0][i][j] = best_value_s;
				OptPolicy_index[0][i][j] = best_policy_index;
			}
			else{
				LoanAction[i][j] = 1;
				NewV[0][i][j] = best_value_takingloan;
				OptPolicy_index[0][i][j] = best_policy_index_takingloan;
			}
		}

	//Company with debt:
	for(int l = 1; l <= maxLoanTime; ++l)
		for(int i = 0; i <= maxC; ++i)
			for(int j = 0; j <= maxA; ++j){
				best_value_s=-DBL_MAX;
				best_policy_index = 0;
				loan_option_calforone = 0;
				for(int policy_index = 0; policy_index < NumPolicy[l][i][l]; ++ policy_index){
					temp_Qvalue = Qvalue (l, i, j, PolicyTable[l][i][j][policy_index], loan_option_calforone);
					if(best_value_s < temp_Qvalue){
						best_value_s = temp_Qvalue;
						best_policy_index = policy_index;
					}
				}
				NewV[l][i][j] = best_value_s;
				OptPolicy_index[l][i][j] = best_policy_index;
			}
}
double DP :: Qvalue(int z, int x, int y, double cash_policy_q, bool loantaking_q){
	sum = 0;
	if(z >= 1){
		loan_outflow = loanPayment;
		loan_income = 0;
	}
	else if (loantaking_q == 1){
		loan_outflow = 0;
		loan_income = loanSize;
	}
	else{
		loan_income = 0;
		loan_outflow = 0;
	};
	for(vector<double>:: size_type it = 0, end =cash_flow.size(); it < end; ++it){
		cash_Q = State_Val(x);
		asset_Q = State_Val(y);
		cash_net = returnRate * asset_Q + cash_flow[it] + loan_income - loanSize;
		sim obj(&cash_Q, &asset_Q, cash_net, cash_policy_q);
		current = obj.getCurrentReward()+loan_income-loan_outflow;
		future = interpolation(z, loantaking_q, cash_Q, asset_Q);
		sum += (current + discR * future) * cash_prob[it];
	}
	return sum;
}
void DP::TestOutPut() {
	policy_OP.open("Policy.csv");
	value_OP.open("Value.csv");
	loan_OP.open("Loan.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			policy_OP << PolicyTable[0][i][j][OptPolicy_index[0][i][j]] << ',';
			value_OP << NewV[0][i][j] << ',';
			loan_OP << LoanAction[i][j] << ',';
		}
		policy_OP << endl;
		value_OP << endl;
		loan_OP << endl;
	}
}
DP :: ~ DP () {
	for (int l = 0; l <= maxLoanTime; ++l){
		for(int i = 0; i <= maxC; ++i){
			for (int j = 0; j <= maxA; ++j){
				delete [] PolicyTable[l][i][j];
			}
			delete [] OldV[l][i];
			delete [] NewV[l][i];
			delete [] NumPolicy[l][i];
			delete [] OptPolicy_index[l][i];
			delete [] PolicyTable [l][i];
		}
		delete [] OldV[l];
		delete [] NewV[l];
		delete [] NumPolicy[l];
		delete [] OptPolicy_index[l];
		delete [] PolicyTable[l];
	}
	delete [] OldV;
	delete [] NewV;
	delete [] NumPolicy;
	delete [] OptPolicy_index;
	delete [] PolicyTable;

	for(int i = 0; i <= maxC; ++ i){
		delete [] LoanAction [i];
	}
	delete [] LoanAction;
}

