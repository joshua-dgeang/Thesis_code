#include <iostream>
#include "simu.cpp"
#include <fstream>


using namespace std;


double CD_mean = 5.0, CD_std = 2.5;
double gaussrand();

int main () {
	srand(31684350);
	char t;
	int Cash_policy[101][301];
	double State_Value[101][301];
	int stageCounter = 0;
	double demand;

	//***************** Read policy file ******************

	ifstream Policy ("Policy.csv");

	if(!Policy.is_open()){
		cerr << "File is NOT open!"<<endl;
	}
	else{
		for(int i = 0; i <= 100; ++i)
			for(int j = 0; j <= 300; ++j){
				Policy >> Cash_policy[i][j] >> t;
			}
	}

	for (int i = 0; i <= 100; ++i)
		Cash_policy[i][0] = 5005;

	State_Value[100][300] = 510.047;

	//____________________________________________________
	
	
	//initialization
	for(int i = 0; i <= 100; ++i)
		for(int j = 0; j <=300; ++j)
			State_Value[i][j] = 0;

	//_______________________________________
	while (stageCounter < 2000000){
		stageCounter ++;
		int StartX = rand()%101, StartY = rand()%301;
		int oldX, oldY;
		bool STOP = 0;
		double CR = 0;
		double alpha = 0.1;
		double gamma = 1.0/1.02;

		while(STOP != 1){
			oldX = StartX;
			oldY = StartY;
			//cout << "STATE: "<<StartX<<":"<<StartY<<":"<<STOP<<endl;
			int action = Cash_policy[StartX][StartY];
			demand = gaussrand();
			//cout << "Action: "<<action<<" Cash demand: "<<demand<<endl;
			simu simulation(&StartX, &StartY, &STOP,demand, action);
			//cout <<"Current Reward: "<<simulation.getCurrentRwd()<<endl;
			CR = simulation.getCurrentRwd();

			State_Value[oldX][oldY] += alpha * (CR + gamma * State_Value[StartX][StartY] - State_Value[oldX][oldY]);
		}

	}

	ofstream Record("Record.csv");
	if(!Record.is_open()){
		cerr<<"Couldn't generate the file!"<<endl;
	}
	else {
		for(int i = 0; i <= 100; ++i){
			for(int j = 0; j <= 300; ++j){
				Record << State_Value[i][j]<<',';
			}
			Record << endl;
		}
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
