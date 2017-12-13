#include <fstream>
#include <cmath>
#include <iostream>
using namespace std;
double TF_I = 1.0;
double TF_II = 2.0;
double TP_I = 0.1;
double TP_II = 0.2;
double TC(double act){
	if (act>0)
		return act*TP_II + TF_II;
	else if (act<0)
		return abs(act)*TP_I + TF_I;
	else
		return 0;
}
int main () {
	const int maxC = 100;
	const int maxA = 300;
	double assetAfter[maxC+1][maxA+1];
	double cashAfter[maxC+1][maxA+1];
	double Action[maxC+1][maxA+1];
	char t;
	ifstream reader("Policy.csv");
	for(int i = 0; i <= maxC; ++i){
		for (int j = 0; j <= maxA; ++j){
			reader >> Action[i][j] >> t;
		}
	}


	for (int i = 0; i <= maxC; ++i)
		for(int j = 0; j <= maxA; ++j){
			double cash = i*1.0;
			double asset = j*1.0;
			double act = Action[i][j];
				cashAfter[i][j] = cash + act - TC(act);
				assetAfter[i][j] = asset -  act;
		}

	ofstream writer_Cash("Cash.csv");
	ofstream writer_Asset("Asset.csv");
	
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			writer_Cash << cashAfter[i][j] << ',';
			writer_Asset << assetAfter[i][j] << ',';

		}
		writer_Cash << '\n';
		writer_Asset << '\n';
	}
}
