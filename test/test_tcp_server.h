/*
 * test_tcp_server.h
 *
 *  Created on: 2016年1月4日
 *      Author: wrb00_000
 */

#ifndef TEST_TCP_SERVER_H_
#define TEST_TCP_SERVER_H_
#include "test_base.h"
#include "cube/service/service.h"
class echo_handler : public cube::service::tcp::handler {
public:
	int on_open(void *arg = 0);
	int on_send(unsigned int sz);
	int on_recv(const void *data, unsigned int sz);
	int on_timeout(time_t tm);
	int on_running(time_t tm);
	int on_close(int err);
};

class test_tcp_server : public test_base{
public:
	int run_test();

private:
	cube::service::tcp::server<echo_handler> _server;
};
#endif /* TEST_TCP_SERVER_H_ */
