/*
 * server.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_SERVER_LINUX_SERVER_H_
#define CUBE_SERVICE_UDP_SERVER_LINUX_SERVER_H_
#include <cube/service/udp/server/linux/worker.h>
#include <vector>
#include <pthread.h>
#include "cube/service/util/socket.h"

namespace cube {
namespace service {
using namespace std;
template<class handler>
class server {
public:
	server();
	virtual ~server();

	/**
	 *	start the udp service binding to @port with local @ip
	 *the @arg will passing the the handler's recall function.
	 *return:
	 *	0--start service success, other --failed
	 */
	int start(unsigned int ip, unsigned short port, unsigned int worknum, void *arg = 0);

	/**
	 *	stop the udp service
	 */
	int stop();

private:
	/**
	 * udp receive thread
	 */
	static void* recv_thread(void* arg = 0);

	/**
	 * process request packet received, just give it to the selected worker
	 */
	void process(dpacket_t *req);

private:
	//local bind socket
	int _socket;
	//local ip and port
	unsigned int _ip;
	unsigned short _port;

	//receive thread handle & id
	pthread_t _thread;
	bool _stop;

	//worker counter
	unsigned int _counter;
	//worker number
	unsigned int _worknum;
	//udp worker list
	vector<worker<handler>*> _workers;
};

template<class handler>
server<handler>::server() :
		_ip(INADDR_ANY), _port(0), _thread(0), _stop(true), _counter(0), _worknum(0) {
}

template<class handler>
server<handler>::~server() {
}

template<class handler>
int server<handler>::start(unsigned int ip, unsigned short port, unsigned int worknum, void *arg/*=NULL*/) {
	/*set the parameter*/
	_ip = ip;
	_port = port;
	_worknum = worknum;


	/*bind local udp socket*/
	_socket = udp_create(_ip, port);
	if (_socket < 0) {
		return -1;
	}

	/*start the workers*/
	if(_worknum < 0){
		return -1;
	}

	for (unsigned int i = 0; i < _worknum; i++) {
		worker<handler> *w = new worker<handler>();
		int err = w->start(_socket, arg);
		if (err != 0)
			return -1;
		_workers.push_back(w);
	}

	/*start the receiver*/
	_stop = false;
	int err = pthread_create(&_thread, 0, recv_thread, this);
	if (err != 0) {
		_stop = true;
		return -1;
	}

	return 0;
}

template<class handler>
int server<handler>::stop() {
	/*stop receiver first*/
	if (!_stop) {
		_stop = true;
		pthread_join(_thread, 0);
	}

	/*stop the workers*/
	for (int i = 0; i < _workers.size(); i++) {
		_workers[i]->stop();
		delete _workers[i];
	}
	_workers.clear();

	return 0;
}

template<class handler>
void server<handler>::process(dpacket_t *req){
	_workers[_counter++%_worknum]->process(req);
}
template<class handler>
void* server<handler>::recv_thread(void *arg) {
	server *pserver = (server *) arg;
	/*!!recvfrom is blocking, so there is problem with stop action!!*/
	while (!pserver->_stop) {
		dpacket_t *req = new dpacket_t(2048);
		socklen_t addrlen = sizeof(req->_addr);
		req->_dsz = ::recvfrom(pserver->_socket, req->_buf, req->_bsz, 0, &req->_addr._addr, &addrlen);
		if (req->_dsz < 0) {
			/*some error occurred for the socket*/
			cout << "fatal error: recvfrom failed." << endl;
			delete req;
			break;
		}

		pserver->process(req);
	}

	pthread_exit(0);
	return 0;
}
}
}

#endif /* CUBE_SERVICE_UDP_SERVER_LINUX_SERVER_H_ */
