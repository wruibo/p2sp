/*
 * worker.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_CLIENT_EPOLL_WORKER_H_
#define CUBE_SERVICE_TCP_CLIENT_EPOLL_WORKER_H_

#include <map>
#include <pthread.h>
#include <sys/epoll.h>
#include "cube/service/util/cdeque.h"
#include "cube/service/util/linux/queue.h"
#include "cube/service/tcp/client/epoll/handler.h"

//for checking the peer shutdown or close events, support from linux kernel 2.6.x
#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif

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
	int start();

	/**
	 * dispatch a new connection peer handler to the worker
	 * @param sock: socket for the connection
	 * @param hd: handler for process the connection
	 * @return:
	 * 	0-success, otherwise for failed
	 */
	void add(handler *hd);

	/**
	 * stop the worker
	 */
	int stop();

private:
	//accept the new connection dispatched from accepter
	void accept();

	//remove a handler with socket @s from handle list
	void remove(int sock);

	//run all handlers
	void run();

	//free all handlers
	void free();

	//iocp worker thread
	static void* work_thread(void *arg);

private:
	//pending handlers waiting for add to epoll
	cdeque<handler*> _pending_handlers;

	//epoll handler
	int _epoll_fd;
	//events number for epoll wait
	int _epoll_wait_events;
	//timeout for epoll wait
	int _epoll_wait_timeout;
	//processing handlers added to the epoll already
	map<int, handler*> _processing_handlers;

	//worker thread handler
	pthread_t _thread_worker;
	//stop flag for worker thread
	bool _worker_stop;
};

worker::worker() :
		_epoll_fd(-1), _epoll_wait_events(32), _epoll_wait_timeout(5), _thread_worker(0), _worker_stop(true){
}

worker::~worker(void) {
}

int worker::start() {
	/*create epoll*/
	_epoll_fd = epoll_create(256);
	if (_epoll_fd == -1)
		return -1;

	/*start worker thread*/
	_worker_stop = false;
	if (pthread_create(&_thread_worker, 0, work_thread, this) != 0) {
		_worker_stop = true;
		return -1;
	}

	return 0;
}

int worker::stop() {
	/*stop worker thread*/
	_worker_stop = true;
	pthread_join(_thread_worker, 0);

	/*close the epoll*/
	if (_epoll_fd != 0)
		close(_epoll_fd);
	_epoll_fd = -1;

	return 0;
}

void worker::add(handler *hd) {
	_pending_handlers.push_back(hd);
}

void worker::accept() {
	handler *hd = 0;
	while(_pending_handlers.pop_front(hd)){
		int sock = hd->sock();
		if (hd->handle_open() != 0) {
			hd->handle_close();
		} else {
			int epoll_opt = EPOLL_CTL_ADD;
			u_int events = EPOLLERR | EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;

			struct epoll_event ev;
			ev.events = events;
			ev.data.ptr = hd;

			if (epoll_ctl(_epoll_fd, epoll_opt, sock, &ev) == -1) {
				hd->handle_error(errno);
				hd->handle_close();
			} else{
				_processing_handlers.insert(pair<int, handler*>(sock, hd));
			}
		}
	}
}

void worker::run() {
	/*get the run time*/
	time_t tm = time(0);

	/*run all handlers*/
	map<int, handler*>::iterator iter = _processing_handlers.begin(), iter_end = _processing_handlers.end();
	while (iter != iter_end) {
		/*check the timer of each handler first*/
		if (iter->second->is_timeout(tm)) {
			int err = iter->second->handle_timeout();
			if (err != 0) {
				iter->second->handle_close();
				_processing_handlers.erase(iter++);
				continue;
			}
		}

		/*recall the handler run of each handler*/
		int err = iter->second->handle_run(tm);
		if (err != 0) {
			iter->second->handle_close();
			_processing_handlers.erase(iter++);
		} else{
			iter++;
		}
	}
}

void worker::free() {
	/*free processing handlers*/
	map<int, handler*>::iterator iter = _processing_handlers.begin(), iter_end = _processing_handlers.end();
	for (; iter != iter_end; iter++) {
		iter->second->handle_close();
	}
	_processing_handlers.clear();

	/*free pending handlers*/
	handler *hd = 0;
	while (_pending_handlers.pop_front(hd)) {
		hd->handle_close();
	}
}

void worker::remove(int sock) {
	map<int, handler*>::iterator iter = _processing_handlers.find(sock);
	if (iter != _processing_handlers.end()) {
		iter->second->handle_close();
		_processing_handlers.erase(iter);
	}
}

void* worker::work_thread(void *arg) {
	worker* worker = (worker*) arg;
	struct epoll_event *events = new epoll_event[worker->_epoll_wait_events];

	while (!worker->_worker_stop) {
		/*accept the new handlers in the pending map*/
		worker->accept();

		/*process the handlers in the epoll*/
		int num = epoll_wait(worker->_epoll_fd, events, worker->_epoll_wait_events, worker->_epoll_wait_timeout);
		for (int i = 0; i < num; i++) {
			int err = 0;
			handler *hd = (handler*) events[i].data.ptr;

			if (err == 0 && events[i].events & EPOLLIN)
				err |= hd->handle_recv();

			if (err == 0 && (events[i].events & EPOLLOUT))
				err |= hd->handle_send();

			if (err == 0 && (events[i].events & EPOLLRDHUP))
				err |= hd->handle_shutd();

			if (err == 0 && (events[i].events & EPOLLERR))
				err |= hd->handle_error(errno);

			if (err != 0)
				worker->remove(hd->sock());
		}

		/*give cpu resource to every handlers in the epoll*/
		worker->run();
	}

	delete[] events;
	worker->free();

	pthread_exit(0);

	return 0;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_WORKER_H_ */
