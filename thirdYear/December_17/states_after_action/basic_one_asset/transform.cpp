#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

double state_value_cash(int);
double state_value_asset(int);
int value_state_cash(double);
int value_state_asset(double);

const int maxC = 100;
const int maxA = 49;
const double TF_I = 1.0;
const double TF_II = 2.0;
const double TP_I = 0.1;
const double TP_II=0.2;

int main () {
	char temp;
	double action[maxC+1][maxA+1];
	int cash[maxC+1][maxA+1];
	//int asset[maxC+1][maxA+1];
	ifstream reader("Policy.csv");
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			reader >> action[i][j] >> temp;
		}
	/*
	ofstream test("Test.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			test << cash[i][j] << ',';
		}
		test << endl;
	}
	*/
	double tran_cos;
	double cash_v;
//	double asset_v;
	for(int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA;++j){
			tran_cos = 0;
			cash_v = state_value_cash(i);
			//asset_v = state_value_asset(j);
			if (action[i][j] < 0){
				tran_cos = TF_I + TP_I * abs(action[i][j]);
				cash_v = cash_v - abs(action[i][j]) - tran_cos;
			//	asset_v += abs(action[i][j]);
			}
			else if (action[i][j] > 0){
				tran_cos = TF_II + TP_II * abs(action[i][j]);
			//	asset_v = asset_v - abs(action[i][j]) - tran_cos;
				cash_v += abs(action[i][j]) - tran_cos;
			}
			else;
			cash[i][j] = value_state_cash(cash_v);
			//asset[i][j] = value_state_asset(asset_v);
		}
	ofstream CashState("CashState.csv");
	//ofstream AssetState("AssetState.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j<=maxA; ++j){
			CashState << cash[i][j] << ',';
			//AssetState << asset[i][j] << ',';
		}
		CashState << endl;
		//AssetState << endl;
	}
	return 0;
}
double state_value_asset(int a){
	return a*1.0;
}
double state_value_cash(int c){
	return c*1.0;
}
int value_state_asset(double a){
	int temp = floor(a+0.5);
	if(temp > maxA)
		return maxA;
	else if(temp < 0)
		return 0;
	return temp;
}
int value_state_cash(double c){
	int temp = floor(c+0.5);
	if(temp > maxC)
		return maxC;
	else if(temp < 0)
		return 0;
	return temp;
}
