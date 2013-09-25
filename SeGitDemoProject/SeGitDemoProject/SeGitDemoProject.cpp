#include <iostream>
#include <string>

using namespace std;



int main(){
	int i = 1, j ;
	system("COLOR 2");
	while (i<100000){
		j = rand() % 10;
		cout << j << " ";

		i+=1;

	}
	cout << "\nMuhahaha!!! Matrix?? :D \n";
	system("pause");

	return 0;
}