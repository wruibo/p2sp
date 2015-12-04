/*
 * work.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_IOCP_WORKER_H_
#define CUBE_SERVICE_IOCP_WORKER_H_
#include <map>
#include "cube/service/util/win/queue.h"
#include "cube/service/tcp/iocp/handler.h"
namespace cube {
namespace service {
using namespace std;

class worker {
public:
	worker(void);
	virtual ~worker(void);

	//stop iocp worker
	int start(int maxconns, int maxevents, int maxwaitm);

	//stop iocp worker
	int stop();

	//dispatch a iocp handler to this worker
	int dispatch(handler *hd);

	//concurrent connections in the worker
	unsigned int concurrency();

private:
	//accept the new connection dispatched from accepter
	int accept();

	//remove a handler with socket @s from handle list
	int remove(SOCKET s);

	//run all handlers
	void run_handlers();

	//free all handlers
	void free_handlers();

	//get handler relate with socket
	handler *get_handler(SOCKET s);

	//reset the concurrency
	void reset_concurrency();

	//iocp worker thread
	static unsigned __stdcall work_thread(void *arg);

private:
	//IOCP handler
	HANDLE _iocp;

	//pending handlers wait for add the iocp
	queue _pendq;

	//handler list in the iocp
	map<SOCKET, handler*> _handlers;

	//worker thread handler
	HANDLE _hdl;
	//worker thread id
	unsigned _thread_id;
	//stop flag for worker thread
	bool _worker_stop;

	//max concurrence connections for the worker
	int _max_conns;
	//max events processed per time for worker
	int _max_events;
	//max wait time for the get iocp status
	int _max_waittm;

	//concurrent connections
	volatile unsigned int _concurrency;
};

typedef enum {
	round, least
} dispatch_t;

}
}

#endif /* CUBE_SERVICE_IOCP_WORKER_H_ */
