

//  Hermite.cpp
//  NewProgramme
//
//  Created by Z.Zhang on 11/15/16.
//  Copyright © 2016 Z.Zhang. All rights reserved.
//

#include "Hermite.hpp"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

Hermite :: Hermite (double mean, double std):Demand_mu(mean),Demand_sigma(std){


    pi = 3.14159265359;
    double xi_initial[9] = {-3.19099,-2.26658,-1.46855,-0.723551,0,0.723551,1.46855,2.26658,3.19099};
    x_vec.reserve(9);
    x_vec.insert(x_vec.begin(), &xi_initial[0], &xi_initial[9]);	

    for (vector<double> :: size_type i = 0, j = x_vec.size(); i < j ; ++i) {
        wi.push_back(Weight(x_vec[i]));
    }
    
    for (vector<double> :: size_type i = 0, j = x_vec.size(); i < j ; ++i) {
        demand.push_back(sqrt(2.0)*Demand_sigma*x_vec[i] + Demand_mu);
        probability.push_back(wi[i]/sqrt(pi));
    }
}

Hermite :: ~ Hermite () {
}

double Hermite :: H_11(int N, double x) {
	if(N==1)
		return 2.0*x;
	else if (N==2)
		return 4.0*pow(x,2.0)-2.0;
	else
		return 2.0*x*H_11(N-1,x)-2.0*(N-1)*H_11(N-2,x);
}


double Hermite :: Weight(double xx) {
	return (pow(2,8)*9*8*7*6*5*4*3*2*sqrt(pi))/(pow(9,2)*pow(H_11(8,xx),2));
}
