/*
 * connector.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_TCP_CONNECTOR_H_
#define CUBE_SERVICE_TCP_TCP_CONNECTOR_H_
#include <list>
#include "cube/service/util/type.h"
#include "cube/service/tcp/tcp_handler.h"
#include "cube/service/tcp/tcp_workers.h"

BEGIN_SERVICE_NS
class tcp_connector{
public:
	tcp_connector();
	virtual ~tcp_connector();

	/**
	 * start the connector
	 *@return:
	 *	0-success, otherwise for failed
	 */
	int start(tcp_workers *workers);

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
	 *	process tracked handlers
	 */
	void process();

	/**
	 * dispatch connected handler to workers
	 */
	void dispatch(handler *hdr);

	/**
	 * free connector resources
	 */
	void free();

	/**
	 * loop for connect thread
	 */
	void loop();

	/**
	 * wait for next loop in connect thread
	 */
	void wait();

	/**
	 * thread function for checking the connection status
	 */
	static __thread_return connect_thread_func(void* arg);

private:
	//workers reference
	tcp_workers *_workers;

	//pending handlers
	std::list<handler*> _pending_handlers;
	mutex_t _pending_handlers_mutex;

	//thread identifier
	thread_t _thread;
	//stop flag for connector
	bool _stop;

};
END_SERVICE_NS
#endif /* CUBE_SERVICE_TCP_EPOLL_CONNECTOR_H_ */
