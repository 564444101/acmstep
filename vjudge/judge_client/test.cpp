#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main()
{
	system("id");
	FILE * fp = fopen("cookie", "w");
	if (fp == NULL) {
		cout << "Open file error!" << endl;
		return 1;
	} else {
		cout <<	"Open file success!" << endl;
	}
	fclose(fp);
	return 0;
}
