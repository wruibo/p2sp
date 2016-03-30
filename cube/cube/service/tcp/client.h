/*
 * connector.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_CLIENT_EPOLL_CLIENT_H_
#define CUBE_SERVICE_CLIENT_EPOLL_CLIENT_H_
#include <cube/service/tcp/epoll/workers.h>
#include <unistd.h>
#include <pthread.h>

#include "cube/service/stdns.h"

BEGIN_SERVICE_TCP_NS
using namespace std;
/*client service using epoll under linux*/
class client {
public:
	client();
	virtual ~client();

	/**
	 * start the client service with specified workers
	 *@param workers: worker number for the service
	 *@return:
	 *	0-success, otherwise failed.
	 */
	virtual int start(int workers);

	/**
	 * build a connection to remote peer
	 *@param ip: remote peer id
	 *@param port: remote peer port
	 *@param hdr: handler for process the connection
	 *@return:
	 *	0-success, otherwise failed.
	 */
	virtual int build(unsigned int ip, unsigned short port, handler *hdr);

	/**
	 * stop the client service
	 *@return:
	 *	0-success, otherwise failed.
	 */
	virtual int stop();

private:
	/**
	 * pull established handlers from connector, dispatch to workers
	 */
	void process_established_handlers();

	/**
	 * wait a tiny time for next loop in client thread
	 */
	void wait_for_next_loop();

	/**
	 * loop for client thread
	 */
	void run_loop();

	/**
	 * thread function for client thread
	 */
	static void* client_thread_func(void *arg);

private:
	//connector of the client
	connector _connector;
	//workers of the client
	workers _workers;

	//thread identifier
	pthread_t _thread;
	//stop flag for client service
	bool _stop;
};
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_CONNECTOR_H_ */
