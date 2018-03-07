#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
double State_to_Value(int state, double discrete);
int Value_to_State(double value, int Boundary, double discrete);
int main () {
	const double DISC = 0.1;
	const double maxC_value = 100;
	const double maxA_value = 300;
	bool grey;
	double Value;
	char t;
	int maxC = Value_to_State(maxC_value, 0, DISC);
	int maxA = Value_to_State(maxA_value, 0, DISC);
	cout << maxC << ":"<<maxA <<endl;
    ifstream Reader("Value.csv");
	ofstream Gray("ValueGray.csv");
	for(int i = 0; i <= maxC; ++i){
		for(int j = 0; j <= maxA; ++j){
			Reader >> Value >> t;
			if(Value < 0){
				grey = 0;
			}
			else {
				grey = 1;
			}
			Gray << grey << ',';
		}
		Gray << endl;
	}
}
double State_to_Value(int s, double d){
	double value = s * d;
	return value;
}
int Value_to_State(double v, int b, double d){
	int state = floor(v/d+0.5);
	if(b == 0){
		return state;
	}
	else {
		if(state > b)
			return b;
		else
			return state;
	}
}

