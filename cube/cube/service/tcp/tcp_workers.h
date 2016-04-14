/*
 * workers.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_WORKERS_H_
#define CUBE_SERVICE_TCP_WORKERS_H_
#include <vector>
#include "cube/service/tcp/tcp_worker.h"
#include "cube/service/tcp/tcp_handler.h"
BEGIN_SERVICE_NS
class tcp_workers{
public:
	tcp_workers();
	virtual ~tcp_workers();

	/**
	 * start workers by specified worker number
	 *@param num: worker number in workers
	 *@return:
	 *	0-success, otherwise for failed.
	 */
	int start(int num, void* arg = 0);

	/**
	 * dispatch a handler to workers
	 */
	int dispatch(socket_t sock, handler *hdr);

	/**
	 * stop workers
	 */
	int stop();

private:
	//worker number
	int _wnum;
	//next dispatch worker position
	int _pos;
	//worker list
	std::vector<tcp_worker*> _workers;
};
END_SERVICE_NS
#endif
