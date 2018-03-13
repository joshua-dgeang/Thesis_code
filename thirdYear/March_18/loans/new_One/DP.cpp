//DP.cpp
#include "DP.hpp"
#include "Hermite.cpp"
#include "sim.cpp"
#include <float.h>

DP :: DP () : maxC_value(100), maxA_value(300), discrete(1.0), smallNum(0.0001), c_mean(-5.0), c_std(5.0), discR(1.0/1.02), action_step_size(1), returnRate(0.05), loanSize(40), loanInterest(0.04), loanTime(30){
	loanPay = loanSize * (loanInterest * pow(1+loanInterest,loanTime)) / (pow(1+loanInterest,loanTime)-1);

	ReadP.open("Policy.csv");
	ReadV.open("Value.csv");
	if(!ReadP.is_open() || !ReadV.is_open()){
		cerr << "Couldn't open the files"<<endl;
		exit(EXIT_FAILURE);
	}

	maxC = maxC_value/discrete;
	maxA = maxA_value/discrete;
	OldV = new double * [maxC+1];
	NewV = new double * [maxC+1];
	OptP = new double * [maxC+1];
	RP = new double * [maxC+1];
	RV = new double * [maxC+1];
	loanAction = new bool * [maxC+1];
	Comparison = new double * [maxC+1];

	for(int i = 0; i <= maxC; ++i){
		OldV[i] = new double [maxA+1];
		NewV[i] = new double [maxA+1];
		OptP[i] = new double [maxA+1];
		RP[i] = new double [maxA+1];
		RV[i] = new double [maxA+1];
		loanAction[i] = new bool [maxA+1];
		Comparison[i] = new double [maxA+1];

		
		for(int j = 0; j <= maxA; ++j){
			ReadP >> RP[i][j] >>t;
			ReadV >> RV[i][j] >>t;
			OldV [i][j] = RV[i][j];
			NewV[i][j] = RV[i][j];;
			OptP[i][j] = 5005;

		}
	}
	//********************************** END OF INITIALIZATION **************************************
	HermiteReader();
	Repitition();
}
void DP :: CalForOneStage (bool T, bool P) {
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			best_policy = 0;
			best_value_s = Qvalue(i, j, 0, T, P);
	//cout << "Here is fine"<<i<<"+"<<j<<endl;
			min_ini = 0;
			max_ini = 0;
			FindAllAction(i,j,&min_ini, &max_ini, T, P);

			for(double policy_x = -min_ini; policy_x <= max_ini; policy_x+=action_step_size){
				if(policy_x < smallNum && policy_x > -smallNum){
				}
				else{
					temp_Qvalue = Qvalue(i, j, policy_x, T, P);
					if(best_value_s < temp_Qvalue){
						best_value_s = temp_Qvalue;
						best_policy = policy_x;
					}
				}
			}
			NewV[i][j] = best_value_s;
			OptP[i][j] = best_policy;
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

double DP :: Qvalue(int x, int y, double ACT, bool T, bool P){
	sum = 0;
	for(vector<double>::size_type it = 0, end = cash_flow.size(); it < end; ++it){
		cash_Q = State_Val_cash(x);
		asset_Q = State_Val_asset(y);
		cash_net = returnRate*asset_Q + cash_flow[it] + loanSize * T - loanPay * P;
		sim obj(&cash_Q, &asset_Q, cash_net, ACT);
		current = obj.getCurrentReward() + loanSize * T - loanPay * P;
		future = interpolation(cash_Q, asset_Q);	
		sum += (current + discR * future) * cash_prob[it];
	}
	return sum;
}
void DP :: Repitition() {
	stage_counter = 1;
	while(stage_counter <= loanTime){
		CalForOneStage(0,0);
		update();
		cout << "Stage: "<<stage_counter <<endl;
		stage_counter++;
	}
	CalForOneStage(0,0);
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			Comparison[i][j] = NewV[i][j];
			NewV[i][j] = RV[i][j];
			OldV[i][j] = RV[i][j];
		}
	stage_counter = 1;
	while(stage_counter <= loanTime){
		CalForOneStage(0,1);
		update();
		cout << "Stage: "<<stage_counter <<endl;
		stage_counter++;
	}
	CalForOneStage(1,0);

	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			if(NewV[i][j] > Comparison[i][j]+smallNum){
				loanAction[i][j] = 1;
			}	
			else{
				loanAction[i][j] = 0;
				NewV[i][j] = Comparison[i][j];
				OptP[i][j] = RP[i][j];
			}
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
	comp.open("Comparison.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			policy_OP << OptP[i][j]<<',';
			value_OP << NewV[i][j]<<',';
			loan << loanAction[i][j] << ',';
			comp << Comparison[i][j] << ',';
		}
		policy_OP << endl;
		value_OP << endl;
		loan << endl;
		comp << endl;
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
		delete [] NewV[i];
		delete [] OptP[i];
		delete [] RP[i];
		delete [] RV[i];
		delete [] Comparison[i];
		delete [] loanAction[i];
	}
	delete [] OldV;
	delete [] NewV;
	delete [] OptP;
	delete [] RP;
	delete [] RV;
	delete [] Comparison;
	delete [] loanAction;
}
