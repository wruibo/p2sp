/*
 * connector.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_CLIENT_IOCP_CONNECTOR_H_
#define CUBE_SERVICE_TCP_CLIENT_IOCP_CONNECTOR_H_

#include <list>
#include <process.h>
#include <Winsock2.h>
#include <Windows.h>

#include "cube/service/stdns.h"
#include "cube/service/error.h"
#include "cube/service/util/socket.h"
#include "cube/service/tcp/client/handler.h"
#include "cube/service/tcp/client/iocp/workers.h"

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
	 * select the writable sockets and error sockets for pending handlers
	 */
	void select(std::list<handler*> *writable_sockets, std::list<handler*> *error_sockets);

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
	static unsigned* connect_thread_func(void* arg);

private:
	//workers reference
	workers *_workers;

	//pending handlers
	std::list<handler*> _pending_handlers;
	CRITICAL_SECTION _pending_handlers_crc;

	//thread identifier
	HANDLE _thread;
	unsigned int _thread_id;
	//stop flag for connector
	bool _stop;

};

connector::connector() : _workers(0), _thread(NULL), _thread_id(0), _stop(true) {
	InitializeCriticalSection(&_pending_handlers_crc);
}

connector::~connector(){
	DeleteCriticalSection(&_pending_handlers_crc)
}

int connector::start(workers *workers){
	if(!_stop){ //connector has been started
		return 0;
	}
	_workers = workers;

	/*start the connector thread*/
	_stop = false;
	_thread = (HANDLE)::_beginthreadex(NULL, 0, connect_thread_func, this, &_thread_id);
	if(_thread == NULL)
	{
		_stop = true;
		return -1;
	}

	return 0;
}

int connector::connect(unsigned int ip, unsigned short port, handler* hdr){
	/*create socket*/
	SOCKET sock = async_tcp_connect(ip, port);
	if(sock == INVALID_SOCKET){
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
	::WaitForSingleObject(_thread, INFINITE);
	::CloseHandle(_thread);

	/*free handler resources*/
	this->free();

	return 0;
}

int connector::track(handler *hdr){
	/*add the handler to pending list*/
	::EnterCriticalSection(&_pending_handlers_crc);
	_pending_handlers.push_back(hdr);
	::LeaveCriticalSection(&_pending_handlers_crc);
	return 0;
}

void connector::untrack(handler *hdr){
	/*remote handler from pending list*/
	::EnterCriticalSection(&_pending_handlers_crc);
	_pending_handlers.remove(hdr);
	::LeaveCriticalSection(&_pending_handlers_crc);
}

void connector::dispatch(handler *hdr){
	if(_workers->dispatch(hdr) != 0){
		hdr->on_close(ERR_DISPATCH_FAILED);
		delete hdr;
	}
}

void connector::select(std::list<handler*> *writable_sockets, std::list<handler*> *error_sockets){
	/*process all pending handlers for connect result testing*/
	::EnterCriticalSection(&_pending_handlers_crc);
	std::list<handler*>::iterator iter = _pending_handlers.begin(), iterend = _pending_handlers.end();
	while(iter != iterend){
		/*fd sets for test connection*/
		fd_set writefds, errorfds;
		FD_ZERO(&writefds);
		FD_ZERO(&errorfds);
		FD_SET((*iter)->sock(), &writefds);
		FD_SET((*iter)->sock(), &errorfds);

		/*fd set limit size reached*/
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		int nfds = ::select(0, 0, &writefds, &errorfds, &tv);
		if(nfds > 0){
			if(FD_ISSET((*iter)->sock(), &writefds)){
				writable_sockets->push_back(*iter);
			}else{
				error_sockets->push_back(*iter);
			}
		}
		iter++;
	}
	::LeaveCriticalSection(&_pending_handlers_crc);
}

void connector::free(){
	::EnterCriticalSection(&_pending_handlers_crc);
	std::list<handler*>::iterator iter = _pending_handlers.begin(), iterend = _pending_handlers.end();
	while(iter != iterend){
		(*iter)->on_close(ERR_TERMINATE_SESSION);
		delete *iter;
		iter++;
	}
	_pending_handlers.clear();
	::LeaveCriticalSection(&_pending_handlers_crc);
}

void connector::process_tracked_handlers(){
	std::list<handler*> writable_sockets, error_sockets;
	this->select(&writable_sockets, &error_sockets);

	/*process the writable sockets*/
	std::list<handler*>::iterator iter = writable_sockets.begin(), iterend = writable_sockets.end();
	while(iter != iterend){
		this->untrack(*iter);
		this->dispatch(*iter);
		iter++;
	}

	/*process the error sockets*/
	iter = error_sockets.begin(), iterend = error_sockets.end();
	while(iter != iterend){
		this->untrack(*iter);
		(*iter)->on_close(ERR_CONNECT_FAILED);
		delete *iter;
		iter++;
	}

}

void connector::wait_for_next_loop() {
	::Sleep(5);
}

void connector::run_loop(){
	while(!_stop){
		/*process the tracked handlers*/
		this->process_tracked_handlers();

		/*wait for the next loop*/
		this->wait_for_next_loop();
	}

	::_endthreadex(0);
}
unsigned* connector::connect_thread_func(void* arg){
	connector* pconnector = (connector*) arg;
	pconnector->run_loop();
	return 0;
}
END_SERVICE_TCP_NS


#endif /* CUBE_SERVICE_TCP_CLIENT_IOCP_CONNECTOR_H_ */
