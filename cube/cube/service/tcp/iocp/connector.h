/*
 * connector.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_IOCP_CONNECTOR_H_
#define CUBE_SERVICE_TCP_IOCP_CONNECTOR_H_
#include <list>
#include <process.h>

#include "cube/service/stdns.h"
#include "cube/service/tcp/handler.h"
#include "cube/service/tcp/iocp/workers.h"

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
	static unsigned __stdcall connect_thread_func(void* arg);

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
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_IOCP_CONNECTOR_H_ */
