#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
using namespace std;
double TC(double);
int main() {
	
	ifstream reader("Policy.csv");
	if(!reader.is_open()) 
		cout << "ERROR"<< endl;
	double policy[101][50];
	double cash_after[101][500];
	string number;
	
	while(reader.good()){
		for (int i = 0; i < 101; ++i){
			for(int j = 0; j < 49; ++j){
				getline(reader, number, ',');
				policy[i][j] = atof(number.c_str());;
			}
			getline(reader,number, '\n');
			policy[i][49] = atof(number.c_str());
		}
	}

	for(int i = 0; i < 101; ++i){
		for(int j = 0; j < 50; ++j){
			double cash = i * 1.0;
			cash_after[i][j] = cash - TC(policy[i][j]) + policy[i][j];
		}
	}
	ofstream output("Cash_After.csv");

	for(int i = 0; i < 101; ++i){
		for(int j = 0; j < 50; ++j){
			output << cash_after[i][j] << ',';
		}
		output<<endl;
	}
}


double TC (double Act){
	if (Act > 0)
		return Act*0.2+2;
	else if (Act < 0)
		return abs(Act)*0.1+1;
	else
		return 0;
}
