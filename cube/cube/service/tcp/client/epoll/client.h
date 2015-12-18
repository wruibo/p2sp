/*
 * connector.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_CLIENT_EPOLL_CLIENT_H_
#define CUBE_SERVICE_CLIENT_EPOLL_CLIENT_H_
#include <list>
#include <vector>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "cube/service/stdns.h"
#include "cube/service/stdsvc.h"
#include "cube/service/tcp/client/epoll/worker.h"
#include "cube/service/tcp/client/epoll/handler.h"

BEGIN_SERVICE_TCP_NS
using namespace std;
/*active tcp connecting service with epoll under linux*/
class client {
public:
	client();
	virtual ~client();

	/*
	 *	start connector with @worker number int epoll trigger mode
	 *return:
	 *	0--success, other--failed.
	 */
	int start(int worker_num);

	/*
	 *	connect to remote peer with @ip:@port in nonblocking mode,
	 *where the @ip&@port are host byte order, with the handler @hd
	 *to process the connection.
	 *@param ip: remote peer ip
	 *@param port: remote peer port
	 *@param hd: handler to process the connection
	 *return:
	 *	0--success, other--failed.
	 */
	int connect(unsigned long ip, unsigned short port, handler *hd);

	/*
	 *	stop connector service
	 *return:
	 *	always return 0
	 */
	int stop();

private:
	/**
	 * free client resources
	 */
	void free();

private:
	//worker number
	int _worker_num;
	//iocp workers for accepter
	vector<worker*> _workers;
	//the next handler process worker
	int _worker_pos;
};

client::client() :
		 _worker_num(0), _worker_pos(0){

}

client::~client() {
}

int client::start(int worker_num) {
	if(worker_num < 1){
		return -1;
	}
	_worker_num = worker_num;

	/*start epoll io workers*/
	for (int i = 0; i < _worker_num; i++) {
		worker *worker = new worker();
		if (worker->start() != 0){
			delete worker;
			free();
			return -1;
		}
		_workers.push_back(worker);
	}

	return 0;
}

int client::connect(unsigned long ip, unsigned short port, handler *hd) {
	/*create socket*/
	int sock = tcp_connect(ip, port);
	if(sock < 0){
		return -1;
	}

	/*set the handler with address information*/
	hd->sock(sock);
	hd->remote_ip(ip);
	hd->remote_port(port);

	/*add the handler to next worker*/
	_workers[_worker_pos++%_worker_num]->add(hd);

	return 0;
}

int client::stop() {
	free();
	return 0;
}

void client::free(){
	for (int i=0; i<_workers.size(); i++) {
		_workers[i]->stop();
		delete _workers[i];
	}
	_workers.clear();
}

END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_CONNECTOR_H_ */
