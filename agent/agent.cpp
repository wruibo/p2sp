#include <iostream>
#include <Windows.h>
using namespace std;

int main(int argc, char *argv[]){
	cout<<"in agent"<<endl;
	void** pvoid = new void*[10];
	int a = 1;
	pvoid[0] = &a;
	cout<<"value :"<<*(int*)pvoid[0]<<endl;
	return 0;
}

