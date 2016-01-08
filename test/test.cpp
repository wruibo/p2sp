#include <string>
#include <iostream>
#include <string.h>
#include "test_tcp_server.h"

static void wait_for_exit() {
	char cmd[256] = {0};
	while(_stricmp(cmd, "exit") != 0){
		std::cout<<"please input exit for stop test: "<<std::endl;
		std::cin>>cmd;
	}
}

int main(int argc, char* argv[]){
	test_tcp_server echoserver;
	echoserver.run_test();

	wait_for_exit();
	return 0;
}
