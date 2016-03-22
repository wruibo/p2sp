#ifndef CUBE_SERVICE_IOCP_WORKER_H_
#define CUBE_SERVICE_IOCP_WORKER_H_
#include <map>
#include <WinSock2.h>
#include <process.h>

#include "cube/service/error.h"
#include "cube/service/tcp/handler.h"
#include "cube/service/util/cdeque.h"
#include "cube/service/tcp/iocp/olpdata.h"

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
	int start(void* arg = NULL);

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
	void remove(SOCKET sock);

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
	static unsigned __stdcall work_thread_func(void *arg);

private:
	//argument will passed to handler
	void* _arg;

	//IOCP handler
	HANDLE _iocp;

	//pending handlers waiting for add to worker
	cdeque<handler*> _pending_handlers;
	//processing handlers in the worker
	map<int, handler*> _processing_handlers;

	//worker thread handler
	HANDLE _thread;
	unsigned int _thread_id;
	//stop flag for worker thread
	bool _stop;
};
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_IOCP_WORKER_H_ */
