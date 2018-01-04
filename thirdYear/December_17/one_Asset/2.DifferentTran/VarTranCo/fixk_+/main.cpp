#include "one_Asset.cpp"
int main () {
	ofstream policy("Policy.csv");
	
	for(int a = 0; a < 30; a+=1){
		cout << a<< endl;
		policy << "-"<<1.0+a*0.2<<"_"<<0.1+a*0.02<<",";
		one_Asset obj(1.0+a*0.2,2.0,0.1+a*0.02,0.2);
		for(int i = 0; i <= one_Asset::MaxC; ++i){
			policy<<obj.PolicyTable[i][obj.OptAction_index[i]]<<',';
		}
		policy << endl;
	}
}
