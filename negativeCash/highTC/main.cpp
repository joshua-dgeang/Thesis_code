#include "one_Asset.cpp"
int main () {
	ofstream policy("Policy.csv");
	
	for(double a = -25; a <= 25.01; a+=0.1){
		cout << a<< endl;
		//policy << "+"<<a*0.2<<"_"<<+a*0.02<<",";
		one_Asset obj(a,1.5,3.0,0.15,0.3);
		for(int i = 0; i <= one_Asset::MaxC; ++i){
			policy<<obj.PolicyTable[i][obj.OptAction_index[i]]<<',';
		}
		policy << endl;
	}
}