#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main () {
	ifstream reader("Policy025.csv");
	ofstream output("deleted.csv");
	double maxC_value=100;
	double maxA_value=300;	
	double action;
	double disc = 0.25;
	char t;
	int maxC = floor(maxC_value/disc);
	int maxA = floor(maxA_value/disc);
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			reader >> action >>t;
			if(i%2 == 0 && j%2 == 0)
				output << action << ',';
		}
		output << endl;
	}

}
