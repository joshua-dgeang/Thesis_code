#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;
int maxA = 300;
int maxC = 100;
double TF_I = 1.0;
double TF_II = 2.0;
double TP_I = 0.1;
double TP_II = 0.2;
int main () {
	double current_cash;
	double action;
	char t;
	double transactionCost;
	
	ifstream Reader("Policy.csv");
	ofstream Gray("Gray.csv");
	ofstream CashLevel("CashLevel.csv");
	ofstream Horizontal("Horizontal.csv");

	for (int flow = 0; flow <= maxA; ++ flow){
		for(int cash = 0; cash <= maxC; ++ cash){
			current_cash = cash *1.0;
			Reader >> action >> t;
			if(action == 0)
				Gray << 1 << ',';
			else
				Gray << 0 << ',';

			if (action > 0){
				transactionCost = TP_II * action + TF_II;
				CashLevel << current_cash + action - transactionCost << ',';
				Horizontal << flow << ',';
				
			}
			else if (action < 0){
				transactionCost = TP_I * abs(action) + TF_I;
				CashLevel << current_cash + action - transactionCost << ',';
				Horizontal << flow << ',';
			}
			else;
		}
		Gray << endl;
	}
}
