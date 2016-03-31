/*
 * connector.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_EPOLL_CONNECTOR_H_
#define CUBE_SERVICE_TCP_EPOLL_CONNECTOR_H_
#include <cube/service/tcp/epoll/workers.h>
#include <cube/service/tcp/tcp_handler.h>
#include <list>
#include <pthread.h>
#include <sys/epoll.h>

#include "cube/service/stdns.h"
#include "cube/service/error.h"
#include "cube/service/util/socket.h"
#include "cube/service/tcp/epoll/events.h"

BEGIN_SERVICE_TCP_NS
class connector{
public:
	connector();
	virtual ~connector();

	/**
	 * start the connector
	 *@return:
	 *	0-success, otherwise for failed
	 */
	int start(workers *workers);

	/**
	 * connect to remote peer with specified handler
	 *@param ip: remote peer ip
	 *@param port: remote peer port
	 *@param hdr: specified handler for the connection
	 *@return:
	 *	0-success, otherwise for failed
	 */
	int connect(unsigned int ip, unsigned short port, handler* hdr);

	/**
	 * stop the connector
	 *@return:
	 *	0-success, otherwise for failed
	 */
	int stop();

private:
	/**
	 *	track connected result for the handler
	 */
	int track(handler *hdr);

	/**
	 * remove handler from track list
	 */
	void untrack(handler *hdr);

	/**
	 * dispatch connected handler to workers
	 */
	void dispatch(handler *hdr);

	/**
	 * free connector resources
	 */
	void free();

	/**
	 *	process tracked handlers
	 */
	void process_tracked_handlers();

	/**
	 * wait for next loop in connect thread
	 */
	void wait_for_next_loop();

	/**
	 * loop for connect thread
	 */
	void run_loop();

	/**
	 * thread function for checking the connection status
	 */
	static void* connect_thread_func(void* arg);

private:
	//workers reference
	workers *_workers;

	//pending handlers
	std::list<handler*> _pending_handlers;
	pthread_mutex_t _pending_handlers_mutex;

	//epoll for check connection status
	int _epoll;
	//events cache for epoll wait
	events _events;

	//thread identifier
	pthread_t _thread;
	//stop flag for connector
	bool _stop;

};

connector::connector() : _workers(0), _pending_handlers_mutex(0), _epoll(-1), _thread(0), _stop(true) {
	pthread_mutex_init(&_pending_handlers_mutex, 0);
}

connector::~connector(){
	pthread_mutex_destroy(&_pending_handlers_mutex);
}

int connector::start(workers *workers){
	if(!_stop){ //connector has been started
		return 0;
	}
	_workers = workers;

	/*create epoll for connector*/
	_epoll = epoll_create(256);
	if(_epoll < 0){
		return -1;
	}

	/*start the connector thread*/
	_stop = false;
	if(pthread_create(&_thread, 0, connect_thread_func, this) != 0)
	{
		_stop = true;
		return -1;
	}

	return 0;
}

int connector::connect(unsigned int ip, unsigned short port, handler* hdr){
	/*create socket*/
	int sock = async_tcp_connect(ip, port);
	if(sock < 0){
		return -1;
	}

	/*set the session property of the handler*/
	hdr->sock(sock);
	hdr->remote_ip(ip);
	hdr->remote_port(port);

	/*track the connect result for the handler*/
	if(this->track(hdr) != 0){
		return -1;
	}

	return 0;
}

int connector::stop(){
	if(_stop){
		return -1;
	}

	/*stop connect thread*/
	_stop = true;
	pthread_join(_thread, 0);

	/*free handler resources*/
	this->free();

	/*close epoll handler*/
	::close(_epoll);

	return 0;
}

int connector::track(handler *hdr){
	/*add the handler to epoll*/
	struct epoll_event ev;
	ev.events = EPOLLOUT|EPOLLET;
	ev.data.ptr = hdr;
	if(epoll_ctl(_epoll, EPOLL_CTL_ADD, hdr->sock(), &ev) < 0){
		return -1;
	}

	/*add the handler to pending list*/
	pthread_mutex_lock(&_pending_handlers_mutex);
	_pending_handlers.push_back(hdr);
	pthread_mutex_unlock(&_pending_handlers_mutex);
	return 0;
}

void connector::untrack(handler *hdr){
	/*remove handler from epoll*/
	epoll_ctl(_epoll, EPOLL_CTL_DEL, hdr->sock(), 0);

	/*remote handler from pending list*/
	pthread_mutex_lock(&_pending_handlers_mutex);
	_pending_handlers.remove(hdr);
	pthread_mutex_unlock(&_pending_handlers_mutex);
}

void connector::dispatch(handler *hdr){
	if(_workers->dispatch(hdr) != 0){
		hdr->on_close(ERR_DISPATCH_FAILED);
		delete hdr;
	}
}

void connector::free(){
	pthread_mutex_lock(&_pending_handlers_mutex);
	std::list<handler*>::iterator iter = _pending_handlers.begin(), iterend = _pending_handlers.end();
	while(iter != iterend){
		(*iter)->on_close(ERR_TERMINATE_SESSION);
		delete *iter;
		iter++;
	}
	_pending_handlers.clear();
	pthread_mutex_unlock(&_pending_handlers_mutex);
}

void connector::process_tracked_handlers(){
	/*wait for epoll event*/
	int num = epoll_wait(_epoll, _events.array(), _events.num(), _events.wait_time());
	for(int i=0; i<num; i++)
	{
		handler *hdr = (handler*)_events.array()[i].data.ptr;
		/*remove the handler from track list*/
		this->untrack(hdr);
		if(_events.array()[i].events & EPOLLOUT){/*connect succeed*/
			/*dispatch the connected handler to workers*/
			this->dispatch(hdr);
		} else {/*connect failed*/
			/*call the handler to deal with error*/
			hdr->on_close(ERR_CONNECT_FAILED);
			/*free the handler*/
			delete hdr;
		}
	}
}

void connector::wait_for_next_loop() {
	::usleep(5000);
}

void connector::run_loop(){
	while(!_stop){
		/*process the tracked handlers*/
		this->process_tracked_handlers();

		/*wait for the next loop*/
		this->wait_for_next_loop();
	}
}
void* connector::connect_thread_func(void* arg){
	connector* pconnector = (connector*) arg;
	pconnector->run_loop();
	pthread_exit(0);
	return 0;
}
END_SERVICE_TCP_NS

#endif /* CUBE_SERVICE_TCP_EPOLL_CONNECTOR_H_ */
