#include "Parameters.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;

double State_to_Value_Asset(int);
double State_to_Value_Cash(int);

int main () {
	double Actions[maxC+1][maxA+1];
	double MoveOnCash[maxC+1][maxA+1];
	double MoveOnAsset[maxC+1][maxA+1];
	bool Grey[maxC+1][maxA+1];
	double TC_Cash;
	double TC_Asset;
	vector<double> cash_target;
	vector<double> asset_target;
	char t;
	ifstream Reader("Policy.csv");

	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			Reader >> Actions[i][j] >> t;
		}
	}
	/*
	ofstream Test("Test.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0 ; j <= maxA; ++j){
			Test << Actions[i][j] << ',';
		}
		Test << endl;
	}
	*/
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			if (Actions[i][j] == 0){
				TC_Cash = 0;
				TC_Asset = 0;
				Grey[i][j] = 0;
			}
			else if (Actions[i][j] > 0){
				TC_Cash = 0; 
				TC_Asset = TF_II + TP_II * abs(Actions[i][j]);
				Grey[i][j] = 1;
			}
			else{
				TC_Cash = TF_I + TP_I * abs(Actions[i][j]);
				TC_Asset = 0;
				Grey[i][j] = 1;
			}
			MoveOnCash[i][j] = Actions[i][j] - TC_Cash;
			MoveOnAsset[i][j] = -Actions[i][j] - TC_Asset;
			if(Actions[i][j] != 0){
			cash_target.push_back(State_to_Value_Cash(i)+MoveOnCash[i][j]);
			asset_target.push_back(State_to_Value_Asset(j)+MoveOnAsset[i][j]);
			}
		}
	//ofstream moveCash("moveCash.csv");
	//ofstream moveAsset("moveAsset.csv");
	ofstream Gray("Gray.csv");
	ofstream Target_C("Target_C.csv");
	ofstream Target_A("Target_A.csv");
	/*
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			if(i%4==0 && j%20==0 && i <= 40 && j <= 200){
				moveCash << MoveOnCash[i][j] << ',';
				moveAsset << MoveOnAsset[i][j] << ',';
			}
		}
		moveCash << endl;
		moveAsset << endl;
	}
	*/
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			Gray << Grey[i][j] << ',';
		}
		Gray << endl;
	}
	for(vector<double>::size_type it = 0, end = cash_target.size(); it < end; ++it){
		Target_C << cash_target[it]<<',';
	}
	for(vector<double>::size_type it = 0, end = asset_target.size(); it < end; ++it){
		Target_A << asset_target[it] <<',';
	}
}
double State_to_Value_Cash(int c){
	return c*1.0;
}
double State_to_Value_Asset(int a){
	return a*1.0;
}
