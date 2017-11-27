//
//  Hermite.hpp
//  NewProgramme
//
//  Created by Z.Zhang on 11/15/16.
//  Copyright © 2016 Z.Zhang. All rights reserved.
//

#ifndef Hermite_hpp
#define Hermite_hpp

#include <vector>
using namespace std;

class Hermite {
public:
    Hermite (double, double);
    ~Hermite();
private:
    const double Demand_mu, Demand_sigma;
    double pi; 
    
    double Weight (double H_4);
    double H_11(int, double x_i);
    vector<double> wi;
    vector<double> x_vec;
    friend class twodimod;
    //friend void TestOutput (Hermite & obj_Name); 
    double expectation;
    vector<double> demand;
    vector<double> probability;
};

#endif /* Hermite_hpp */
