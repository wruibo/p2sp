/*
 * test_tcp_server.cpp
 *
 *  Created on: 2016年1月4日
 *      Author: wrb00_000
 */
#include "test_tcp_server.h"

int echo_handler::on_open(void *arg) {
	return -1;
}

int echo_handler::on_send(unsigned int sz) {
	return -1;
}


int echo_handler::on_recv(const void* data, unsigned int sz) {
	return -1;
}

int echo_handler::on_timeout(time_t tm) {
	return -1;
}

int echo_handler::on_running(time_t tm) {
	return -1;
}

int echo_handler::on_close(int err) {
	return -1;
}

int test_tcp_server::run_test() {
	_server.start(8000, 1);
	return 0;
}
