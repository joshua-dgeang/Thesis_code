#include "one_Asset.cpp"
int main () {
	ofstream policy("Policy.csv");
	
	for(int a = 0; a < 10; a+=1){
		cout << a<< endl;
		policy << "K-"<<a*0.5<<",";
		one_Asset obj(a*0.5,0,0,0);
		for(int i = 0; i <= one_Asset::MaxC; ++i){
			policy<<obj.PolicyTable[i][obj.OptAction_index[i]]<<',';
		}
		policy << endl;
	}
}
