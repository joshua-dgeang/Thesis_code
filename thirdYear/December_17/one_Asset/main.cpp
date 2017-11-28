#include "one_Asset.cpp"
int main () {
	ofstream policy("Policy.csv");
	
	for(double aa = -10.0; aa <= 10.0; aa+=0.1){
		cout << "$$$$$$$$$$$$$$$$$$$$$$ "<<aa<<" $$$$$$$$$$$$$$$$$$$"<<endl;
		policy << "aa:"<<aa<<',';
		one_Asset obj(aa);
		for(int i = 0; i <= one_Asset::MaxC; ++i){
			policy << obj.PolicyTable[i][obj.OptAction_index[i]]<<',';
		}
		policy << endl;
	}
}
