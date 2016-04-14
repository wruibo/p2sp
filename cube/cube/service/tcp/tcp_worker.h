/*
 * worker.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_EPOLL_WORKER_H_
#define CUBE_SERVICE_TCP_EPOLL_WORKER_H_
#include <map>
#include "cube/service/util/type.h"
#include "cube/service/tcp/tcp_handler.h"

BEGIN_SERVICE_NS
using namespace std;
class tcp_worker {
public:
	tcp_worker();
	virtual ~tcp_worker(void);

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
	void dispatch(socket_t sock, handler *hd);

	/**
	 * stop the worker
	 */
	int stop();

private:
	/**
	 * accept the new connection dispatched from connector or accepter
	 */
	void accept_new_handlers();

	/**
	 * process the active handlers for io operation
	 */
	void process_active_handlers();

	/**
	 * give run chance for active handler
	 */
	void run_active_handlers();

	/**
	 * remove a handler with socket @s from handle list
	 */
	void remove_active_handler(socket_t sock);

	/**
	 * free all handlers
	 */
	void free_all_handlers();

	/**
	 * loop for work thread
	 */
	void run_work_loop();

	/**
	 * thread function for worker
	 */
	static __thread_return work_thread_func(void *arg);

private:
	//argument will be passed to handler
	void* _arg;

	//io mode structure
	io_mode_t _io_mode;

	//pending handlers waiting for add to worker
	std::map<socket_t, handler> _new_handlers;
	mutex_t _new_handlers_mutex;

	//active handlers in the worker
	std::map<socket_t, handler*> _active_handlers;

	//worker thread handler
	thread_t _thread;
	//stop flag for worker thread
	bool _stop;
};
END_SERVICE_NS
#endif /* CUBE_SERVICE_TCP_WORKER_H_ */
