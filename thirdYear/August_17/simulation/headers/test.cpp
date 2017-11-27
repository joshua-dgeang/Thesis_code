#include "simu.cpp"
#include <iostream>
using namespace std;
int main () {
	int cash = 100;
	int asset = 300;
	double dmand = 5.75;
	int action = 0;
	bool stop = 0;
	cout << cash << endl;
	cout << asset << endl;
	simu obj( &cash, &asset, &stop, dmand, action);
	cout << "**************"<<endl;
	cout << cash << endl;
	cout << asset << endl;
	cout <<endl;
	cout << obj.getCash_simu()<<':'<<obj.getAsest_simu()<<endl;
	cout << obj.getCurrentRwd()<<endl;

}
