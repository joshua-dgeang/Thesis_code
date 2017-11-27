#include "sarsaL.hpp"
#include <iostream>
using namespace std;
SarsaL :: SarsaL ():CD_mean(5.0), CD_std(2.5),Max_C(100), Max_A(300),ALPHA(0.1),disC(1.0/1.02),LAMBDA(0.8) {
	srand(31684350);
	head = NULL;
	curr = NULL;
	output.open("Result.csv");
	test.open("Test.csv");

	twoDptr = new statePointer * [Max_C+1];
	for(int i = 0; i <= Max_C; ++i){
		twoDptr[i] = new statePointer [Max_A+1];
		for(int j = 0; j<= Max_A; ++j)
			twoDptr[i][j] = new stateValue;
	}
	fileReader();
	for(int i = 0; i <= Max_C;++i)
		for(int j = 0; j <= Max_A;++j){
			twoDptr[i][j]->Qvalue = 0;
			twoDptr[i][j]->Zvalue = 0;
		}
	twoDptr[100][300]->Qvalue = 510.047;
	RepeatEpisode();

}
SarsaL :: ~ SarsaL () {
	for(int i = 0; i <= Max_C; ++i){
		for(int j = 0; j <= Max_A; ++j){
			delete twoDptr[i][j];
		}
		delete [] twoDptr [i];
	}
	delete [] twoDptr;

}
void SarsaL :: RepeatEpisode () {
	EpisodeCounter = 0;
	MaxEpisode = 100000;
	while (EpisodeCounter < MaxEpisode){

		EpisodeCounter ++;
		//********* STARTING POINT **************
		S_x = 10;
		S_y = 95;
		stp = 0;
		//______________________________________
		sarsa_lambda(S_x, S_y, stp);

		EndofEpisode();	

		if(EpisodeCounter % 1 == 0){
			output << twoDptr[S_x][S_y] -> Qvalue <<endl;
			cout << "EPISODE: "<<EpisodeCounter<< endl;
		}
	}
}

void SarsaL :: fileReader() {
	Policy.open("Policy.csv");
	test.open("Test.csv");
	if(!Policy.is_open()){
		cerr << "File is NOT open!"<<endl;
	}
	else{
		for(int i = 0; i <= Max_C; ++i)
			for(int j = 0; j <= Max_A; ++j){
				Policy >> twoDptr[i][j] -> OptimalAction >> t;
			}

		for (int i = 0; i <= Max_C; ++i)
			twoDptr[i][0]->OptimalAction = 5005;
	}

	/*
	   for(int i = 0; i <= 100; ++i){
	   for(int j = 0; j <= 300; ++j){
	   test << twoDptr[i][j] -> OptimalAction <<',';
	   }
	   test << endl;
	   }
	   */
}

double SarsaL :: gaussrand(){
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
void SarsaL :: sarsa_lambda(int sx, int sy, bool STP){
	head = twoDptr[sx][sy];
	bool new_STP = STP;
	int new_sx = sx;
	int new_sy = sy;
	while(new_STP == 0){
		int old_sx = sx;
		int old_sy = sy;
		demand = gaussrand();
		simu obj(&new_sx, &new_sy, &new_STP, demand, twoDptr[new_sx][new_sy]->OptimalAction);
		cout << new_STP<<endl;
		cout << new_sx << ":"<<new_sy<<endl;
		DELTA = obj.getCurrentRwd() + disC*(twoDptr[new_sx][new_sy]->Qvalue) - twoDptr[old_sx][old_sy]->Qvalue;
		twoDptr[old_sx][old_sy]->Zvalue+=1;
		repeat = false;
		curr = head;
		//******************
		//going through the current list. Comparing with the state sx,sy. If it is not new, record it.
		while(curr != NULL){
			if(curr == twoDptr[new_sx][new_sy]){
				repeat = true;
			}
			curr = curr->next;
		}
		//if the state sx,sy is new, add it to the end of the list.
		if(repeat == false){
			curr = head;
			while(curr -> next != NULL){
				curr = curr -> next;
			}
			curr -> next = twoDptr[new_sx][new_sy];
		}
		//going through the list, update all the states in the list.
		while(curr != NULL){
			curr->Qvalue += ALPHA * DELTA * (curr ->Zvalue);
			curr->Zvalue *= disC * LAMBDA;
			curr = curr -> next;
		};
		//******************
	}
}
void SarsaL :: EndofEpisode(){
	curr = head;
	while(curr!=NULL){
		curr->Zvalue = 0;
		curr = curr -> next;
		head->next = NULL;
		head = curr;
	}
}

