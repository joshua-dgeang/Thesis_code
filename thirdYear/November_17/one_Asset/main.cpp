#include "one_Asset.cpp"
int main () {
	one_Asset obj;
	for(int i = 0; i <= 100; ++i){
		for(int j = 0; j < obj.totalNumber_of_Actions[i]; j++){
			cout << obj.PolicyTable[i][j] << ' ';
		}	
		cout << endl;
	}

}
