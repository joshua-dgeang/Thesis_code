#include "simu.cpp"
#include <iostream>
#include <fstream>
using namespace std;
double CD_std = 2.5;
double CD_mean = 5.0;
double gaussrand ();
int main () {
	srand(31684350);
	int Policy[101][301];
	ifstream oj;
	ofstream test("simu10_80.csv");
	oj.open("policy.csv");
	char t;
	for(int i = 0; i <= 100; ++i){
		for(int j = 0; j <= 300; ++j){
			oj >> Policy[i][j] >> t;
		}
	}

	for(int i = 0; i <= 100; ++i){
		Policy[i][0] = 5005;	
	}


	int simulation = 0;
	while (simulation <= 100){
		simulation++;

		test<<"simulation_"<<simulation<<',';

	double cash = 10.0;
	double asset = 120.0;
	int timeCounter = 0;
	int X,Y;
	int action;
	while (timeCounter < 100){
		timeCounter++;
		cash = (cash >= 100 ? 100:cash);
		asset = (asset >= 300 ? 300:asset);
		test << cash+asset <<',';
		cout << "HERE "<<endl;
		
		bool stop = 0;
		double demand = gaussrand();
		X = floor(cash+0.5);
		Y= floor(asset+0.5);
		action = Policy[X][Y];
		simu obj(&cash, &asset, &stop, demand, action);
	}
	test << endl;
	}





}
double gaussrand(){
		static double V1, V2, S;
		static int phase = 0;                              
		double X;                                      
		if(phase == 0){                                    
			do{                                            
				double U1 = (double) rand() / RAND_MAX;
			        double U2 = (double) rand() / RAND_MAX;
				V1 = 2 * U1 - 1;                       
			        V2 = 2 * U2 - 1;                       
			     	S = V1 * V1 + V2 * V2;                 
			}while(S >= 1 || S == 0);                      
			X = V1 * sqrt (-2 * log(S)/S);                 
		} else                                             
			X = V2 * sqrt( -2 * log(S)/S);                 
	phase = 1 - phase;                                 
	return X * CD_std  + CD_mean;                      
}                                                      
