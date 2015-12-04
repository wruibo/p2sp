/*
 * worker.h
 *
 *  Created on: 2015年12月1日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_CLIENT_EPOLL_WORKER_H_
#define CUBE_SERVICE_UDP_CLIENT_EPOLL_WORKER_H_
#include <map>
#include <sys/epoll.h>
#include "cube/service/udp/client/handler.h"

namespace cube{
namespace service{
namespace udp{

class worker{
public:
	worker();
	virtual ~worker();

	/**
	 * start the worker
	 */
	int start();

	/**
	 * stop the worker
	 */
	int stop();

	/**
	 * add a task to the worker
	 */
	int add(int socket, handler *hd);

private:
	static void* work_thread(void* arg);

private:
	//file descriptor for epoll
	int _efd;

	//handlers for udp peer
	std::map<int, handler*> _handlers;

	//worker thread
	pthread_t _thread;
	bool _stop;

};

worker::worker():_efd(-1), _thread(0), _stop(true){

}

worker::~worker(){

}

int worker::start(){
	/*create epoll*/
	_efd = epoll_create(16);
	if(_efd < 0){
		return -1;
	}

	/*start the worker thread*/
	_stop = false;
	if(pthread_create(&_thread, 0, work_thread, this) != 0){
		::close(_efd);
		_stop = true;
		return -1;
	}

	return 0;
}

int worker::stop(){
	if(_stop){
		return 0;
	}

	/*stop work thread first*/
	_stop = true;
	pthread_join(_thread, 0);

	/*clear the handlers*/
	_handlers.clear();

	return 0;
}

int worker::add(int socket, handler* hd){

	return 0;
}

void* worker::work_thread(void* arg){
	worker* pworker = (worker*)arg;
	while(!pworker->_stop){

	}

	return 0;
}

}
}
}

#endif /* CUBE_SERVICE_UDP_CLIENT_EPOLL_WORKER_H_ */
