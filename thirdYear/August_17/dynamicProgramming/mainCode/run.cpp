#include <iostream>
#include <fstream>
#include "TwoDiMod.cpp"
#include <vector>
using namespace std;
void TestOutput(twodimod &obj);
int main () {
	twodimod obj;
	TestOutput(obj);
}
void TestOutput(twodimod &obj){
	ofstream valueoutput("TwD_value.csv");                          
	ofstream policyoutput("TwD_policy.csv");                        
	ofstream bankruptcyoutput("TwD_bankruptcy.csv");                
	if(!valueoutput.is_open()){                                     
		cout << "error with value output."<<endl;               
	        exit(EXIT_FAILURE);                                     
	}                                                               
	if(!policyoutput.is_open()){                                    
		cout << "error with policy output."<<endl;              
	        exit(EXIT_FAILURE);                                     
	}                                                               
	if(!bankruptcyoutput.is_open()){                                
	        cout << "error with bankruptcy output."<<endl;          
	        exit(EXIT_FAILURE);                                     
	}                                                               
	else;                                                           
	for(int i = 0; i <= obj.MaxC; ++i){                          
		for(int j = 0; j <= obj.MaxA; ++j){                 
		        valueoutput << obj.NewValue[i][j] << ',' ;      
			policyoutput << obj.OptPolicy[i][j] << ',' ;   
			bankruptcyoutput << obj.Bankruptcy[i][j] << ',';
		}                                                       
		valueoutput << '\n';                                    
	        policyoutput << '\n';                                   
	        bankruptcyoutput <<'\n';                                
	}                                                               
}
