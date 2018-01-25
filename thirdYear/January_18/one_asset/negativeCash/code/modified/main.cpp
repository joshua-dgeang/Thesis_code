#include "one_Asset.cpp"
int main () {
	ofstream policy("Policy.csv");
	
	for(int a = 0; a < 1; a+=1){
		cout << a<< endl;
		//policy << "k-"<<a*0.05<<"_k+"<<a*0.1<<",";
		one_Asset obj(1.0,2.0,0.1,0.2);//K-, K+, k-, k+
		for(int i = 0; i <= one_Asset::MaxC; ++i){
			policy<<obj.PolicyTable[i][obj.OptAction_index[i]]<<',';
		}
		policy << endl;
	}
}
