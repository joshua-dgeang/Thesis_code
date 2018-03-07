#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
double State_to_Value(int state, double discrete);
int Value_to_State(double value, int Boundary, double discrete);
int main () {
	const double DISC = 1;
	const double maxC_value = 300;
	const double maxA_value = 300;
	const double TF_I = 1.0;
	const double TF_II = 2.0;
	const double TP_I = 0.1;
	const double TP_II = 0.2;
	//const double returnRate = 0.05;
	double TransactionCost;
	double c_move, a_move;
	double C_justMove, A_justMove;
	bool grey;
	double Action;
	char t;
	int maxC = Value_to_State(maxC_value, 0, DISC);
	int maxA = Value_to_State(maxA_value, 0, DISC);
	cout << maxC << ":"<<maxA <<endl;
	ifstream Reader("Policy.csv");
	ofstream cmove("C_move.csv");
	ofstream amove("A_move.csv");
	ofstream cmove_up("C_move_up.csv");
	ofstream amove_up("A_move_up.csv");
	ofstream Table_cmove("C_move_table.csv");
	ofstream Table_amove("A_move_table.csv");
	ofstream Gray("Gray.csv");
	Table_cmove << "NAME"<<',';
	Table_amove << "NAME"<<',';
	for(int j = 0; j <= maxA; ++j){
		Table_cmove << "Asset_"<<State_to_Value(j,DISC)<<',';
		Table_amove << "Asset_"<<State_to_Value(j,DISC)<<',';
	}
	Table_cmove << endl;
	Table_amove << endl;
	for(int i = 0; i <= maxC; ++i){
		Table_cmove << "Cash_"<<State_to_Value(i, DISC)<<',';
		Table_amove << "Cash_"<<State_to_Value(i, DISC)<<',';

		for(int j = 0; j <= maxA; ++j){
			Reader >> Action >> t;
			if(Action < 0.0001 && Action > -0.0001){
				c_move = 0;
				a_move = 0;
				C_justMove = 0;
				A_justMove = 0;
				grey = 0;
			}
			else if (Action >= 0.0001){
				TransactionCost = TF_II + TP_II * abs(Action);
				c_move = State_to_Value(i,DISC) + Action;
				a_move = State_to_Value(j,DISC) - Action - TransactionCost;
				C_justMove = Action ;
				A_justMove = - Action - TransactionCost;
				cmove << c_move << endl;
				amove << a_move << endl;
				
				grey = 1;

			}
			else if (Action <= -0.0001){
				TransactionCost = TF_I + TP_I * abs(Action);
				c_move = State_to_Value(i,DISC) + Action - TransactionCost;
				a_move = State_to_Value(j,DISC) - Action;
				C_justMove = Action - TransactionCost;
				A_justMove = - Action;
				grey = 1;
				cmove_up << c_move << endl;
				amove_up << a_move << endl;
			}
			Table_cmove << C_justMove << ',';
			Table_amove << A_justMove << ',';
			Gray << grey << ',';

		}
		Table_cmove << endl;
		Table_amove << endl;
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

