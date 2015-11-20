/*
 * worker.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_EPOLL_WORKER_H_
#define CUBE_SERVICE_EPOLL_WORKER_H_

#include <map>
#include <pthread.h>
#include <sys/epoll.h>
#include "cube/service/util/linux/queue.h"
#include "cube/service/tcp/epoll/handler.h"

//for checking the peer shutdown or close events, support from linux kernel 2.6.x
#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif


namespace cube{
namespace service{
	using namespace std;
	class worker
	{
	public:
		worker();
		virtual ~worker(void);

		//stop epoll worker
		int start(int maxconns, int maxevents, int maxwaitm);

		//stop epoll worker
		int stop();

		//dispatch a epoll handler to this worker
		int dispatch(handler *hd);

		//concurrent connections in the worker
		unsigned int concurrency();

	private:
		//accept the new connection dispatched from accepter
		int accept();

		//remove a handler with socket @s from handle list
		int remove(int sock);

		//run all handlers
		void run_handlers();

		//free all handlers
		void free_handlers();

		//reset the concurrency
		void reset_concurrency();

		//iocp worker thread
		static void* work_thread(void *arg);

	private:
		//epoll handler
		int _epoll_fd;

		//pending handlers wait for add the epoll
		queue _pendq;

		//handler list in the epoll
		map<int, handler*> _handlers;

		//worker thread handler
		pthread_t _thread_worker;
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

	typedef enum{round, least}dispatch_t;
}
}
#endif /* CUBE_SERVICE_EPOLL_WORKER_H_ */
