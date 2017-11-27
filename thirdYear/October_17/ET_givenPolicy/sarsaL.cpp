#include "sarsaL.hpp"
#include <iostream>
using namespace std;
sarsaL :: sarsaL () : CD_mean(5.0), CD_std(2.5), Max_C(100), Max_A (300), alpha(0.1), discR(1.0/1.02), lambda(0.6) {
	Vvalue = new double * [Max_C+1];
	OptAct = new int * [Max_C+1];
	//Zvalue = new int * [Max_C+1];

	for (int i = 0; i <= Max_C; ++i){
		Vvalue[i] = new double [Max_A+1];
		OptAct[i] = new int [Max_A+1];
		//Zvalue[i] = new int [Max_A+1];
		for(int j = 0; j <= Max_A; ++j){
			Vvalue[i][j] = 0.0;
			//Zvalue[i][j] = 0;
		}
	}
	Vvalue[Max_C][Max_A] = 510.047;

	fileReader();
	Repeat();
}
sarsaL :: ~ sarsaL(){
	for (int i = 0; i <= Max_C; ++i){
		delete [] Vvalue [i];
		delete [] OptAct [i];
		//delete [] Zvalue [i];
	}
	delete [] Vvalue;
	delete [] OptAct;
	//delete [] Zvalue;
}

void sarsaL :: fileReader () {
	Policy.open("Policy.csv");	
	Reco.open("Result.csv");
	//Record.open("Logo.txt");

	//test.open("test.csv");
	if(!Policy.is_open()){
		cerr << "File is NOT is_open!"<<endl;
	}
	else {
		for(int i = 0; i <= Max_C; ++i){
			for(int j = 0; j <= Max_A; ++j){
				Policy >> OptAct[i][j] >> t;
			}
			OptAct[i][0] = 5005;
		}
	}

	/*
	for (int i = 0; i <= Max_C; ++i){
		for(int j = 0; j <= Max_A; ++j){
			test << OptAct[i][j] <<',';
		}
		test << endl;
	}
	*/

}

double sarsaL :: gaussrand(){                                  
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

void sarsaL :: Repeat () {
	EpisodeCounter = 0;
	MaxEpisode = 500000;
	while(EpisodeCounter < MaxEpisode){

		EpisodeCounter ++;
		//*************** STARTING POINT *********************
		stateX = 10;
		stateY = 100;
		stp = 0;
		if(EpisodeCounter % 1000 == 0){
			cout << "We are at: "<<EpisodeCounter << endl;

			Reco << Vvalue[stateX][stateY]<<endl;
		}

		//====================================================
		Record << "State: "<<stateX <<':'<<stateY<<endl;
		sarsa_lambda();
		EndOfEpisode();
	}
}

void sarsaL :: sarsa_lambda() {
	// Create the first node of the list, then point head at it
	head = new linkedList;
	head->S_x = stateX;
	head->S_y = stateY;

	head->Qvalue = Vvalue[stateX][stateY];
	head->Zvalue = 0;
	head->OptimalAction = OptAct[stateX][stateY];
	head->next = NULL;
	Record << "->Now the head pointer pointing at "<<stateX<<':'<<stateY<<endl;
	//************************ end of initialization ********************
	while(stp == 0){
		old_sx = stateX;
		old_sy = stateY;
		demand = gaussrand();
		Record<<"Demand: "<<demand<<endl;
		simu obj(&stateX, &stateY, &stp, demand, OptAct[stateX][stateY]);
		DELTA = obj.getCurrentRwd() + discR * Vvalue[stateX][stateY] - Vvalue[old_sx][old_sy];
		
		Record<<"\told state: "<<old_sx<<':'<<old_sy<<endl;
		Record<<"\tnew state: "<<stateX<<':'<<stateY<<endl;
		Record <<"\tcurrent reward:" <<obj.getCurrentRwd();
		Record <<"\tValues: new:old: "<<Vvalue[stateX][stateY]<<':'<<Vvalue[old_sx][old_sy]<<endl;
		Record<<"\tDelta: "<<DELTA<<endl;
		//go through the lisk till end, update its Zvalue:
		curr = head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->Zvalue += 1;
		Record <<"\tUpdate the z value for state: "<<curr->S_x<<","<<curr->S_y<<endl;
		Record<<"\twhich now is equal to "<<curr->Zvalue<<endl;
		//=================================================
		Record<<"go through the current list. Comparing with the state sx, sy. If it is not new, record it."<<endl;
		repeat = false;
		curr = head;
		while (curr != NULL){
			if(curr->S_x == old_sx && curr -> S_y ==old_sy){
				repeat = true;
				curr->Zvalue += 1;
				Record << "\t\tIt repeats the list"<<endl;
			}
			curr = curr->next;
		}
		AddNewNode(repeat);
		Record<<"go through the list, update all the states in the list;"<<endl;
		curr = head;
		while(curr != NULL){
			curr->Qvalue += alpha * DELTA * (curr -> Zvalue);
			curr->Zvalue *= discR * lambda;
			Record <<"-> State: "<<curr->S_x<<':'<<curr->S_y<<endl;
			Record <<"-> Qvalue and Zvalue: "<<curr->Qvalue<<':'<<curr->Zvalue<<endl;
			curr = curr -> next;
		}
	}
}

void sarsaL :: AddNewNode (bool rpt){
	if(rpt == false){
		Record<<"The states "<<old_sx<<','<<old_sy<<" doesn't repeat, add it at the end of the list"<<endl;
		LinkPtr NEW = new linkedList;
		NEW -> S_x = old_sx;
		NEW -> S_y = old_sy;
		NEW -> Qvalue = Vvalue[old_sx][old_sy];
		NEW -> Zvalue = 1;
		NEW -> OptimalAction = OptAct[old_sx][old_sy];
		NEW -> next = NULL;
		curr = head;
		while(curr -> next != NULL)
			curr = curr->next;
		curr->next = NEW;
	}
}
void sarsaL :: EndOfEpisode () {
	curr = head;
	while(curr != NULL){
		Vvalue[curr->S_x][curr->S_y] = curr->Qvalue;
		curr = curr -> next;
		delete head;
		head = curr;
	}
}
