#include "sim.cpp"
#include <iostream>
using namespace std;
int main () {
	double cash_level = 10;
	double asset_level = 10;
	double cash_demand = -5;
	double action = 0;
	sim obj(&cash_level, &asset_level, cash_demand, action);
	cout << obj.getCurrentReward()<<endl;
	cout << cash_level<<':'<<asset_level<<endl;
	return 0;

}
