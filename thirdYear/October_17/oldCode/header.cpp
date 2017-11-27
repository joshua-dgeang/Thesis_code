	//header.cpp
#include "header.hpp"
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
Draft :: Draft ():maximumEpisode(10000000),Max_C(100), Max_A(300), ReturnRate(0.05), TF_I(1.0), TF_II(2.0), TP_I(0.1), TP_II(0.2), CD_mean(5.0), CD_std(2.5), ShorPen(10.0), discR(1.0/1.02),ALPHA(0.1),EPSILON(0.1), LAMBDA(0.5),start_cash(10),start_asset(100){
	head = NULL;
	curr = NULL;
	temp = NULL;
	srand(31684350);
	Result.open("Record.csv");
	//Result << "Programme beginning..."<<endl;
	
	//Initialising multi-dimensional arrays:

	threeDptr = new statePointer * [Max_C+1];

    for(int i = 0; i <= Max_C; ++i){
        threeDptr[i]= new statePointer [Max_A+1];
        for(int j = 0; j <= Max_A; ++j)
            threeDptr[i][j] = new stateValue;
    }

    fileReader();
    for(int i = 0; i <= Max_C;++i)
        for(int j = 0; j <= Max_A;++j){
            threeDptr[i][j]->Qvalue = 0;
            threeDptr[i][j]->Zvalue = 0;
        }
    threeDptr[100][300]->Qvalue = 510.047;
    RepeatEpisode();
}
Draft :: ~ Draft () {
	cout << "Succeed!"<<endl;
	for(int i = 0; i <= Max_C; ++i){
		for(int j = 0; j <= Max_A; ++j){
			delete threeDptr[i][j];
		}
		delete [] threeDptr[i];
	}
	delete [] threeDptr;
}

void Draft :: fileReader() {
    Policy.open("Policy.csv");

    if(!Policy.is_open()){
        cerr << "File is NOT open!"<<endl;
    }
    else{
        for(int i = 0; i <= Max_C; ++i)
            for(int j = 0; j <= Max_A; ++j){
                Policy >> threeDptr[i][j] -> CashAction >> t;
            }
        
        for (int i = 0; i <= Max_C; ++i)
            threeDptr[i][0]->CashAction = 5005;
    }
}
void Draft :: RepeatEpisode () {
	EpisodeCounter = 0;
	while(EpisodeCounter < maximumEpisode){
		EpisodeCounter++;
		cout << "This is episode "<<EpisodeCounter<<endl;
		//Result << "**************** EPISODE "<<EpisodeCounter <<" ******************"<<endl;
		sarsa_lambda();
		EndOfEpisode();
		if(EpisodeCounter%1000 == 0){
			cout << "EPISODE: "<<EpisodeCounter<<endl;
			Result << threeDptr[start_cash][start_asset]->Qvalue<<endl;
		}

	}
}
void Draft :: test() {
	for(int i = 0; i <= 29; ++i)
		cout << gaussrand() <<'\t';
}
void Draft :: sarsa_lambda () {//Just one episode:
	S_x = start_cash;
	S_y = start_asset;

	bankruptcy = 0;
	terminal = 0;
	head = threeDptr[S_x][S_y];

	while (bankruptcy == 0 && terminal == 0){
		//Result << "AT A NEW STEP: "<<endl;
		//***** SIMULATION ******
		cash_os = S_x * 1.0 + ReturnRate * S_y;
		asset_os = 1.0 * S_y;
		//Result << "We current state is: "<<S_x<<','<<S_y<<endl;
		action_os = threeDptr[S_x][S_y] -> CashAction;
		if(action_os == 5005){
			//Result << "Declared bankruptcy..."<<endl;
			//Result << "This Episode should end here"<<endl;
			bankruptcy = 1;
			currentReward = 0.0;
			S_x_next = 0;
			S_y_next = 0;
		}
		else {
			if(action_os < 0){
				tranCst = TF_I + TP_I * abs(action_os);
				cash_os = cash_os - abs(action_os) - tranCst;
				asset_os = asset_os + abs(action_os);
			}
			else if (action_os > 0){
				tranCst = TF_II + TP_II * abs(action_os);
				cash_os += abs(action_os);
				asset_os = asset_os - tranCst - abs(action_os);
			}
			else
				tranCst = 0;
			//Result << "The cash action is: "<<action_os<<" with transaction cost "<<tranCst<<endl;
			cash_demand = gaussrand();
			//Result << "The cash demand is: "<<cash_demand<<endl;
			cash_os = cash_os - cash_demand;
			//Result << "Now the current cash is: "<<cash_os<<endl;
			if(cash_os < 0){
				//Result << "Cash shortage occurs..."<<endl;
				cost_by_shortage = ShorPen + TF_II + TP_II * abs(cash_os);
				cash_os = 0;
				asset_os = asset_os-cost_by_shortage - abs(cash_os);
				//Result << "The cost is "<<cost_by_shortage<<" and now the current cash/asset is "<<cash_os<<'/'<<asset_os<<endl;
			}
			else 
				cost_by_shortage = 0;
			currentReward = ReturnRate * S_y - tranCst - cash_demand - cost_by_shortage;
			//Result << "The currentReward R is: "<<currentReward <<endl;
			if(cash_os <= 0 && asset_os <= 0){
				//Result <<"Company is forced to bankrupty! Episode should end here!"<<endl;
				bankruptcy = 1;
				S_x_next = 0;
				S_y_next = 0;
			}
			else if (cash_os >= Max_C && asset_os >= Max_A){
				//Result << "Company is in the ideal position. Episode should end here!"<<endl;
				terminal = 1;
				S_x_next = Max_C;
				S_y_next = Max_A;
			}
			else {
				S_x_next = floor(cash_os + 0.5);
				S_y_next = floor(asset_os + 0.5);
				if(S_x_next > Max_C)
					S_x_next = Max_C;
				if(S_y_next > Max_A)
					S_y_next = Max_A;
			}
		}
				//Result << "The next state is: "<<S_x_next <<'/'<<S_y_next<<'/'<<A_next<<endl;
		//******* END OF SIMULATION *****
		DELTA = currentReward + discR * (threeDptr[S_x_next][S_y_next]->Qvalue) - (threeDptr[S_x][S_y] -> Qvalue);
		threeDptr[S_x][S_y] -> Zvalue += 1;
		//Result << "++++++++++++ DELTA is "<<DELTA <<" and the z value for state ("<<S_x<<'/'<<S_y<<'/'<<A<<") is "<<threeDptr[S_x][S_y][A] -> Zvalue<<endl;
		// *******************************
		Repeat = false;
		curr = head;
		//Result<< "Go through the list: "<<endl;
		//Result << "From Head  "<<endl;
		while(curr != NULL){
			if(curr == threeDptr[S_x_next][S_y_next]){
					Repeat = true;
			}
			curr = curr->next;
		}
		if(Repeat == false){
			//cout << S_x_next << ','<<S_y_next<<','<<A_next<<endl;
			//cout << threeDptr[S_x_next][S_y_next][A_next]->Qvalue<<','<<threeDptr[S_x_next][S_y_next][A_next]->Zvalue<<endl;
			curr = head;
			while(curr -> next != NULL){
				curr = curr -> next;
			}
			//Result <<"Not Repeated, a new state, added to the list"<<endl;
			curr->next  = threeDptr[S_x_next][S_y_next];

		}
		while (curr!=NULL){
		curr->Qvalue += ALPHA * DELTA * (curr->Zvalue);
      		curr->Zvalue *= discR * LAMBDA;
		curr = curr -> next;		
		}
		//**********************************
		S_x = S_x_next;
		S_y = S_y_next;
	}
	//End of the episode;
	EndOfEpisode();
}

double Draft :: gaussrand(){
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
void Draft :: EndOfEpisode () {
	//Result <<"At the end of the episode, set all Z value to 0 and break all the link!"<<endl;
	curr = head;
	while (curr!=NULL){
		curr->Zvalue = 0;
		curr = curr -> next;
		head -> next = NULL;
		head = curr;
	}
}
