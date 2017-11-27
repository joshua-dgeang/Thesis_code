//simu.cpp
//this class is used to simulate cash problem for one stage;
//users should pass three pointers, two int pointers to the current state and one bool pointer to the stop condition and two parameters (cash demand, cash action);
//this class will return the currentReward in this stage as well as the cash value and the asset value at the end this stage.
//Please DO NOT pass a simulation with stop condtion equal to 1!!!
//this class will also modify the original state to the state in next stage.
#include "simu.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
const double simu :: ReturnRate = 0.05;
const double simu :: TF_I = 1.0;
const double simu :: TF_II = 2.0;
const double simu :: TP_I = 0.1;
const double simu :: TP_II = 0.2;
const double simu :: ShorPen = 10.0;
const double simu :: discR = 1.0/1.02;
simu :: simu (int * const S_x, int * const S_y, bool* const stop, double d, int t): S_c(S_x),S_a(S_y),stp(stop),cash_demand(d),cash_transfer(t){
	if(*stp == 1){
		cerr << "Simulation Should Have Stopped!!!"<<endl;
	}
	simulation();
}

double simu :: getCurrentRwd(){
	return currentReward;
}
double simu :: getCash_simu(){
	return cash_simu;
}
double simu :: getAsest_simu(){
	return asset_simu;
}
bool simu :: getSTP(){
	return *stp;
}
void simu :: simulation () {
	cash_simu = *S_a * ReturnRate + *S_c * 1.0;
	asset_simu = *S_a * 1.0;
	if(cash_transfer == 5005){//declare bankruptcy
		*stp = 1;
		currentReward = 0.0;
		*S_c = 0;
		*S_a = 0;
	}
	else {
		if(cash_transfer < 0){
			cost_by_trans = TF_I + TP_I * fabs(cash_transfer);
			cash_simu = cash_simu - fabs(cash_transfer) - cost_by_trans;
			asset_simu += fabs(cash_transfer);
		}
		else if (cash_transfer > 0){
			cost_by_trans = TF_II + TP_II * fabs(cash_transfer);
			asset_simu = asset_simu - cost_by_trans - fabs(cash_transfer);
			cash_simu += fabs(cash_transfer);
		}
		else
			cost_by_trans = 0;
	cash_simu = cash_simu - cash_demand;
	if(cash_simu < 0){
		cost_by_shortage = ShorPen + TF_II + TP_II * abs(cash_simu);
		asset_simu = asset_simu - cost_by_shortage - abs(cash_simu);
		cash_simu = 0;
	}
	else
		cost_by_shortage = 0;
	currentReward = ReturnRate * *S_a - cost_by_trans - cash_demand - cost_by_shortage;

	if(cash_simu <= 0 && asset_simu <= 0){
		*stp = 1;
		*S_c = 0;
		*S_a = 0;
	}
	if(cash_simu > 100)
		cash_simu = 100;
	if(asset_simu >300)
		asset_simu = 300;
	if(cash_simu >= 100 && asset_simu >= 300){
		*stp = 1;
		*S_c = 100;
		*S_a = 300;
	}
	else {
		
		*S_c = floor(cash_simu+0.5);
		*S_a = floor(asset_simu+0.5);
	}
}
}


