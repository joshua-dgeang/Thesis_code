#include "sim.cpp"
int main () {
	double cash_level = 10.0;
	double asset_level = 100.0;
	double flow = -3.5;
	double action = -1.5;
	cout << "cash/asset/demand/action: "<<endl;
	cout << cash_level <<':'<< asset_level<<':'<<flow<< ':'<<action<<endl;
	
	sim obj( &cash_level, &asset_level, flow, action);
	cout <<"Result: "<<endl<<"Current Reward: "<<obj.getCurrentReward()<<endl;
	cout <<"State: "<< cash_level <<':'<<asset_level<<endl;
}
