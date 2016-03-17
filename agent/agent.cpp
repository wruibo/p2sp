#include <iostream>
#include <process.h>
using namespace std;

static unsigned __stdcall client_thread_func(void* arg){
	cout<<"in client thread..."<<endl;
	return 0;
}


int main(int argc, char *argv[]){
	::_beginthreadex(NULL, 0, client_thread_func, 0, 0, 0);

	cout<<"in agent"<<endl;
	void** pvoid = new void*[10];
	int a = 1;
	pvoid[0] = &a;
	cout<<"value :"<<*(int*)pvoid[0]<<endl;
	return 0;
}
