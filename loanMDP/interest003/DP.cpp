//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include "sim.cpp"
#include <float.h>

DP :: DP () : maxC_value(100), maxA_value(300), discrete(1.0), smallNum(0.0001), c_mean(-5.0), c_std(5.0), discR(1.0/1.02), action_step_size(1), returnRate(0.05), loanSize(40), loanInterest(0.03), loanTime(30){
	loanPay = loanSize * (loanInterest * pow(1+loanInterest,loanTime)) / (pow(1+loanInterest,loanTime)-1);


	maxC = maxC_value/discrete;
	maxA = maxA_value/discrete;

	OldV = new double ** [maxC+1];
	NewV = new double ** [maxC+1];
	OptP = new double ** [maxC+1];
	loanAction = new bool * [maxC+1];

	for(int i = 0; i <= maxC; ++i){
		OldV[i] = new double * [maxA+1];
		NewV[i] = new double * [maxA+1];
		OptP[i] = new double * [maxA+1];
		loanAction[i] = new bool [maxA+1];
		
		for(int j = 0; j <= maxA; ++j){
			loanAction[i][j] = 0;
			OldV[i][j] = new double [loanTime+1];
			NewV[i][j] = new double [loanTime+1];
			OptP[i][j] = new double [loanTime+1];
			for(int k = 0; k <= loanTime; ++k){
				OldV[i][j][k] = 0;
				NewV[i][j][k] = 0;
				OptP[i][j][k] = 5005;
			}
		}
	}
	//********************************** END OF INITIALIZATION **************************************
	HermiteReader();
	Repitition();
}
void DP :: CalForOneStage () {
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j)
			for(int k = 0; k <= loanTime; ++k){
				if (k >= 1){
					best_policy = 0;
					best_value_s = Qvalue(i,j,k, 0, 0, 1);
					min_ini = 0;
					max_ini = 0;
					FindAllAction(i,j,&min_ini, &max_ini, 0, 1);
					for(double policy_x = -min_ini; policy_x <= max_ini; policy_x+=action_step_size){
						if(policy_x < smallNum && policy_x > -smallNum){
						}
						else{
							temp_Qvalue = Qvalue(i, j, k, policy_x, 0, 1);
							if(best_value_s < temp_Qvalue){
								best_value_s = temp_Qvalue;
								best_policy = policy_x;
							}
						}
					}
					NewV[i][j][k] = best_value_s;
					OptP[i][j][k] = best_policy;
				}
				else{
					//Taking loan...
					best_policy_loan = 0;
					best_value_loan = Qvalue(i, j, k, 0, 1, 0);
					min_ini = 0;
					max_ini = 0;
					FindAllAction(i,j,&min_ini, &max_ini, 1, 0);
					for(double policy_x = -min_ini; policy_x <= max_ini; policy_x+=action_step_size){
						if(policy_x < smallNum && policy_x > -smallNum){
						}
						else{
							temp_Qvalue = Qvalue(i, j,k, policy_x, 1, 0);
							if(best_value_loan < temp_Qvalue){
								best_value_loan= temp_Qvalue;
								best_policy_loan = policy_x;
							}
						}
					}
					//Not Taking loan..

					best_policy = 0;
					best_value_s = Qvalue(i, j, k, 0, 0, 0);
					min_ini = 0;
					max_ini = 0;
					FindAllAction(i,j,&min_ini, &max_ini, 0, 0);
					for(double policy_x = -min_ini; policy_x <= max_ini; policy_x+=action_step_size){
						if(policy_x < smallNum && policy_x > -smallNum){
						}
						else{
							temp_Qvalue = Qvalue(i, j, k, policy_x, 0, 0);
							if(best_value_s < temp_Qvalue){
								best_value_s = temp_Qvalue;
								best_policy = policy_x;
							}
						}
					}
					//Comparison...
					if(best_value_loan > best_value_s+smallNum){
						loanAction[i][j] = 1;
						OptP[i][j][k] = best_policy_loan;
						NewV[i][j][k] = best_value_loan;
					}
					else {
						loanAction[i][j] = 0;
						OptP[i][j][k] = best_policy;
						NewV[i][j][k] = best_value_s;
					}
				}
		}
}
void DP :: FindAllAction(int x, int y, double* min, double* max, bool ToN, bool PoN){

	cash_cur = State_Val_cash(x);
	asset_cur = State_Val_asset(y);
	cash_cur = cash_cur +  returnRate * asset_cur + ToN * loanSize - PoN * loanPay;
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
}
void DP :: HermiteReader() {
	Hermite reader (c_mean, c_std);
	for(vector<double> :: size_type it = 0, end = reader.demand.size(); it < end; ++it){
		cash_flow.push_back(reader.demand[it]);
		cash_prob.push_back(reader.probability[it]);
	}
}
double DP :: interpolation (double x, double y, int l, bool t) {
	if(l >= 1){
		if(x > State_Val_cash(maxC)) x = State_Val_cash(maxC);
		if(y > State_Val_asset(maxA)) y = State_Val_asset(maxA);//x and y are the cash/asset values. Here to make sure that they don't exceed the maximum value of this model.
		if(x < 0) x = 0;
		if(y < 0) y = 0;
		scf=floor(x/discrete);
		saf=floor(y/discrete);
		scc=ceil(x/discrete);
		sac=ceil(y/discrete);

		VA = OldV[scf][saf][l-1];
		VB = OldV[scc][saf][l-1];
		VC = OldV[scc][sac][l-1];
		VD = OldV[scf][sac][l-1];
		VF = (x/discrete-scf) * (VB - VA) + VA;
		VE = (x/discrete-scf) * (VC - VD) + VD;
		VG = (y/discrete-saf) * (VE -VF) + VF;
		return VG;
	}
	else if (t == 1){
		if(x > State_Val_cash(maxC)) x = State_Val_cash(maxC);
		if(y > State_Val_asset(maxA)) y = State_Val_asset(maxA);//x and y are the cash/asset values. Here to make sure that they don't exceed the maximum value of this model.
		if(x < 0) x = 0;
		if(y < 0) y = 0;
		scf=floor(x/discrete);
		saf=floor(y/discrete);
		scc=ceil(x/discrete);
		sac=ceil(y/discrete);

		VA = OldV[scf][saf][loanTime];
		VB = OldV[scc][saf][loanTime];
		VC = OldV[scc][sac][loanTime];
		VD = OldV[scf][sac][loanTime];
		VF = (x/discrete-scf) * (VB - VA) + VA;
		VE = (x/discrete-scf) * (VC - VD) + VD;
		VG = (y/discrete-saf) * (VE -VF) + VF;
		return VG;
	}
	else{
		if(x > State_Val_cash(maxC)) x = State_Val_cash(maxC);
		if(y > State_Val_asset(maxA)) y = State_Val_asset(maxA);//x and y are the cash/asset values. Here to make sure that they don't exceed the maximum value of this model.
		if(x < 0) x = 0;
		if(y < 0) y = 0;
		scf=floor(x/discrete);
		saf=floor(y/discrete);
		scc=ceil(x/discrete);
		sac=ceil(y/discrete);

		VA = OldV[scf][saf][l];
		VB = OldV[scc][saf][l];
		VC = OldV[scc][sac][l];
		VD = OldV[scf][sac][l];
		VF = (x/discrete-scf) * (VB - VA) + VA;
		VE = (x/discrete-scf) * (VC - VD) + VD;
		VG = (y/discrete-saf) * (VE -VF) + VF;
		return VG;
	}
}

double DP :: Qvalue(int x, int y, int l, double ACT, bool T, bool P){
	sum = 0;
	for(vector<double>::size_type it = 0, end = cash_flow.size(); it < end; ++it){
		cash_Q = State_Val_cash(x);
		asset_Q = State_Val_asset(y);
		cash_net = returnRate*asset_Q + cash_flow[it] + loanSize * T - loanPay * P;
		sim obj(&cash_Q, &asset_Q, cash_net, ACT);
		current = obj.getCurrentReward() + loanSize * T - loanPay * P;
		future = interpolation(cash_Q, asset_Q, l, T);	
		sum += (current + discR * future) * cash_prob[it];
	}
	return sum;
}
void DP :: Repitition() {
	stage_counter = 1;
	while(STP == 0){
	//while(stage_counter <= 1){
		CalForOneStage();
		STP = Comparison();
		update();
		cout << "Stage: "<<stage_counter <<endl;
		stage_counter++;
	}
	TestOutPut();
}
double DP :: State_Val_cash(int c){
	return c * discrete;
}
double DP :: State_Val_asset(int a){
	return a * discrete;
}
void DP :: TestOutPut(){
	policy_OP.open("Policy_loan.csv");
	value_OP.open("Value_loan.csv");
	loan.open("loanAction.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			policy_OP << OptP[i][j][0]<<',';
			value_OP << NewV[i][j][0]<<',';
			loan << loanAction[i][j] << ',';
		}
		policy_OP << endl;
		value_OP << endl;
		loan << endl;
	}
}
void DP :: update(){
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j)
			for(int k = 0; k <= loanTime; ++k){
				OldV[i][j][k] = NewV[i][j][k];
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

bool DP :: Comparison () {
	maxdifference = 0.0;
	for (int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j)
			for(int k = 0; k <= loanTime; ++k){
				difference = abs (NewV[i][j][k] - OldV[i][j][k]);
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
DP :: ~ DP () {
	for (int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			delete [] OldV[i][j];
			delete [] NewV[i][j];
			delete [] OptP[i][j];
		}
		delete [] OldV[i];
		delete [] NewV[i];
		delete [] OptP[i];
		delete [] loanAction[i];
	}
	delete [] OldV;
	delete [] NewV;
	delete [] OptP;
	delete [] loanAction;
}
