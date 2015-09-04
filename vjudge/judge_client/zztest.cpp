#include "global.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
	
	//FILE *file = fopen("/home/vjudge/run0/tmpfiles/data.html","w");
	string tmp = fileToString("/home/vjudge/run0/tmpfiles/data.html");
	cout<<"zztest"<<endl;
	cout<<tmp<<endl;
	return 0;
}
