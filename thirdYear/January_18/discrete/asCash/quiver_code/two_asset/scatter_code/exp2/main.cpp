#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;

double State_to_Value_Asset(int);
double State_to_Value_Cash(int);
double TransactionCost(double);
int maxC = 500;
int maxA = 1500;
double TF_I = 1.0;
double TF_II = 2.0;
double TP_I = 0.1;
double TP_II = 0.2;

int main () {
	double cash, asset;
	double action;
	double TC_Cash;
	double TC_Asset;
	char t;
	ifstream Reader("Policy.csv");
	ofstream TargetC("TargetCash.csv");
	ofstream TargetA("TargetAsset.csv");
	ofstream Gray("Gray.csv");

	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			cash = State_to_Value_Cash(i);
			asset = State_to_Value_Asset(j);
			Reader >> action >> t;
			if(action == 0)
				Gray << 0 << ',';
			else
				Gray << 1 << ',';


			//if (i%5==0 && j%5 == 0){
			if(action > 0){
				TC_Cash = cash + action;
				TC_Asset = asset - action - TransactionCost(action);;
				TargetC << TC_Cash << ',';
				TargetA << TC_Asset << ',';
			}
			else if (action < 0 ){
				TC_Cash = cash + action - TransactionCost(action);
				TC_Asset = asset - action;
				TargetC << TC_Cash << ',';
				TargetA << TC_Asset << ',';
			}
			else;
			//}
		}
		Gray << endl;
	}
}

double State_to_Value_Cash(int c){
	return c * 0.2;
}
double State_to_Value_Asset(int a){
	return a * 0.2;
}
double TransactionCost(double action){
	if (action > 0 )
		return TP_II * action + TF_II;
	else if (action < 0)
		return TP_I * abs(action) + TF_I;
	else
		return 0;
}
