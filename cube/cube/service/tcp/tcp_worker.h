/*
 * worker.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_EPOLL_WORKER_H_
#define CUBE_SERVICE_TCP_EPOLL_WORKER_H_
#include <cube/service/tcp/tcp_handler.h>
#include <map>
#include <time.h>
#include <pthread.h>
#include <sys/epoll.h>

#include "cube/service/util/cdeque.h"
#include "cube/service/tcp/epoll/events.h"

BEGIN_SERVICE_TCP_NS
using namespace std;
class worker {
public:
	worker();
	virtual ~worker(void);

	/**
	 *start the worker for processing the connected peers
	 *@return
	 *	0-success, otherwise for failed
	 */
	int start(void* arg = 0);

	/**
	 * dispatch a new connection peer handler to the worker
	 * @param sock: socket for the connection
	 * @param hd: handler for process the connection
	 * @return:
	 * 	0-success, otherwise for failed
	 */
	void dispatch(handler *hd);

	/**
	 * stop the worker
	 */
	int stop();

private:
	/**
	 * remove a handler with socket @s from handle list
	 */
	void remove(int sock);

	/**
	 * free all handlers
	 */
	void free();

	/**
	 * accept the new connection dispatched from connector
	 */
	void accept_pending_handlers();

	/**
	 * poll all processing handlers from epoll, process io operation
	 */
	void poll_processing_handlers();

	/**
	 * process timeout event & give run for every processing handler
	 */
	void run_processing_handlers();

	/**
	 * loop for work thread
	 */
	void run_loop();

	/**
	 * thread function for worker
	 */
	static void* work_thread_func(void *arg);

private:
	//argument will be passed to handler
	void* _arg;

	//epoll handle
	int _epoll;
	//epoll events cache
	events _events;

	//pending handlers waiting for add to worker
	cdeque<handler*> _pending_handlers;
	//processing handlers in the worker
	map<int, handler*> _processing_handlers;

	//worker thread handler
	pthread_t _thread;
	//stop flag for worker thread
	bool _stop;
};

worker::worker(): _arg(0), _epoll(-1), _thread(-1), _stop(true){
}

worker::~worker(void) {
}

int worker::start(void* arg/* = 0*/) {
	/*set the argument*/
	_arg = arg;

	/*create epoll*/
	_epoll = epoll_create(256);
	if (_epoll == -1)
		return -1;

	/*start worker thread*/
	_stop = false;
	if (pthread_create(&_thread, 0, work_thread_func, this) != 0) {
		_stop = true;
		return -1;
	}

	return 0;
}

void worker::dispatch(handler *hd) {
	_pending_handlers.push_back(hd);
}

int worker::stop() {
	if(_stop){
		return -1;
	}

	/*stop worker thread*/
	_stop = true;
	pthread_join(_thread, 0);

	/*free all handlers*/
	this->free();

	/*close the epoll*/
	::close(_epoll);

	return 0;
}

void worker::remove(int sock) {
	map<int, handler*>::iterator iter = _processing_handlers.find(sock);
	if (iter != _processing_handlers.end()) {
		_processing_handlers.erase(iter);
	}
}

void worker::free() {
	/*free processing handlers*/
	map<int, handler*>::iterator iter = _processing_handlers.begin(), iterend = _processing_handlers.end();
	while(iter != iterend){
		iter->second->on_close(ERR_TERMINATE_SESSION);
		delete *iter;
		iter++;
	}
	_processing_handlers.clear();

	/*free pending handlers*/
	handler *hdr = 0;
	while (_pending_handlers.pop_front(hdr)) {
		hdr->on_close(ERR_TERMINATE_SESSION);
		delete hdr;
	}
}

void worker::accept_pending_handlers() {
	handler *hdr = 0;
	while(_pending_handlers.pop_front(hdr)){
		if (hdr->on_open(_arg) != 0) {
			/*recall on open failed*/
			hdr->on_close(ERR_TERMINATE_SESSION);
			delete hdr;
		} else {
			/*add the handler to epoll*/
			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
			ev.data.ptr = hdr;

			if (epoll_ctl(_epoll, EPOLL_CTL_ADD, hdr->sock(), &ev) < 0) {
				hdr->on_close(ERR_EPOLL_ADD_FAILED);
				delete hdr;
			} else{
				_processing_handlers.insert(pair<int, handler*>(hdr->sock(), hdr));
			}
		}
	}
}

void worker::poll_processing_handlers() {
	/*process the handlers in the epoll*/
	int num = epoll_wait(_epoll, _events.array(), _events.num(), _events.wait_time());
	for (int i = 0; i < num; i++) {
		unsigned int event = _events.array()[i].events;
		handler *hdr = (handler*) _events.array()[i].data.ptr;

		if(event & EPOLLERR){
			hdr->on_close(ERR_EPOLL_ERROR);
			this->remove(hdr->sock());
			delete hdr;
		}else{
			if(event & EPOLLIN) {
				/*receive data until there is not data could be received*/
				void* data = 0;
				unsigned int recv_sz = 0;
				while(hdr->redo_recv(&data, &recv_sz)) {
					if(hdr->on_recv(data, recv_sz) != 0){
						hdr->on_close(ERR_TERMINATE_SESSION);
						this->remove(hdr->sock());
						delete hdr;
						break;
					}

					/*reset receive data buffer*/
					delete data;
					data = 0;
					recv_sz = 0;
				}
			}

			if(event & EPOLLOUT){
				/*send data until there is no data could be sent*/
				unsigned int send_sz = 0;
				while(hdr->redo_send(&send_sz)) {
					if(hdr->on_send(send_sz) != 0) {
						hdr->on_close(ERR_TERMINATE_SESSION);
						this->remove(hdr->sock());
						delete hdr;
						break;
					}
				}
			}
		}
	}
}

void worker::run_processing_handlers() {
	/*get the run time*/
	time_t now = ::time(0);

	/*run all handlers*/
	map<int, handler*>::iterator iter = _processing_handlers.begin(), iter_end = _processing_handlers.end();
	while (iter != iter_end) {
		handler *hdr = (handler*)iter->second;
		/*check the timer of each handler first*/
		if (hdr->is_timeout(now)) {
			if (hdr->on_timeout(now) != 0) {
				hdr->on_close(ERR_TERMINATE_SESSION);
				_processing_handlers.erase(iter++);
				delete hdr;
				continue;
			}
		}

		/*recall the handler run of each handler*/
		if (hdr->on_running(now) != 0) {
			hdr->on_close(ERR_TERMINATE_SESSION);
			_processing_handlers.erase(iter++);
			delete hdr;
			continue;
		} else{
			iter++;
		}
	}
}

void worker::run_loop(){
	while (!_stop) {
		/*accept the new handlers in the pending map*/
		this->accept_pending_handlers();

		/*poll events from epoll and process io operation*/
		this->poll_processing_handlers();

		/*run every processing handler, inlcude timer operation*/
		this->run_processing_handlers();
	}
}

void* worker::work_thread_func(void *arg) {
	worker* pworker = (worker*) arg;
	pworker->run_loop();
	pthread_exit(0);
	return 0;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_WORKER_H_ */
