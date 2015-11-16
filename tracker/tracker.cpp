#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void* thread_fun(void *arg){
	int num = 0;
	while(true){
		cout<<"in thread function "<<num<<endl;
		num++;
		usleep(1000000);
	}
}

int main(int argc, char *argv[]){
	pthread_t thread;
	pthread_create(&thread, 0, thread_fun, 0);

	int num = 0;
	while(true){
		cout<<"in main function "<<num<<endl;
		num++;
		usleep(1000000);
	}
	return 0;
}
