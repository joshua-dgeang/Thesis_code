#include "one_Asset.cpp"
int main () {
	ofstream policy("Policy.csv");
	
	for(int a = 0; a < 10; a+=1){
		cout << a<< endl;
		policy << "k-"<<0.1+a*0.05<<",";
		one_Asset obj(1.0,2.0,0.1+a*0.05,0.2);
		for(int i = 0; i <= one_Asset::MaxC; ++i){
			policy<<obj.PolicyTable[i][obj.OptAction_index[i]]<<',';
		}
		policy << endl;
	}
}
