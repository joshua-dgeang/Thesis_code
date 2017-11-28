#include "one_Asset.cpp"
int main () {
	double X = 2.5;
	one_Asset obj;
	cout << X <<','<<-4.3<<','<<-1.6<<endl;
	cout <<	obj.simulation(&X, -4.3, -1.6) << ',' << X << endl;
	
	}
