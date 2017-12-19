#include "GaussRand.cpp"
#include <iostream>
#include <fstream>
using namespace std;
const int MaxC = 200;
const double TF_I = 0.5;
const double TF_II = 1.0;
const double TP_I = 0.05;
const double TP_II=0.1;
const double startingPoint = 0;
const double SCR = 2.0;
const int MaxStage = 200;
double Policy[MaxC+1];
void Reading();
double TC (double);
double StageToValue(int);
int ValueToState(double);
int main () {
	GaussRand randnum(0.0, 5.0, 3133);
	Reading();
	double cash = startingPoint;
	double policy;
	double SP;
	double flow;
	int stageCounter = 0;
	ofstream writer("Sim.csv");


	while(stageCounter < MaxStage){
		SP = 0;
		flow = randnum.GetRN();
		
		stageCounter++;
		if(cash > StageToValue(MaxC))
		       cash = startingPoint;//don't think this will really happen
		else if (cash < 0)
			cash = startingPoint;//won't happen either

		policy = Policy[ValueToState(cash)];
		//after action:
		writer << cash << endl;
		cash = cash + policy - TC(policy);
		writer << cash << endl;
		//after demand:
		cash += flow;
		//writer << cash << endl;
		//check for cash deficit:
		if(cash < 0){
			SP = abs(cash)+SCR*abs(cash);
		}
		else 
			SP = 0;
		cash = cash - SP;
		//writer << cash << endl;
	}
}
void Reading(){
	ifstream reader("Policy.csv");;
	if(!reader.is_open()){
		cout << "Can't open file"<<endl;
		exit(EXIT_FAILURE);
	}
	char t;
	for(int i = 0; i <= MaxC; ++i){
		reader >> Policy[i] >> t;		
	}
}
double TC(double act){
	if(act > 0)
		return act*TP_II + TF_II;
	else if (act < 0)
		return abs(act)*TP_I + TF_I;
	else
		return 0;
}
int ValueToState(double value){
	int s = floor(value*5.0 + 0.5);
	if(s > MaxC)
		s = MaxC;
	else if (s <0)
		s = 0;
	return s;
}
double StageToValue(int state){
	return state * 0.2;
}
