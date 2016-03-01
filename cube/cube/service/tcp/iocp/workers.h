/*
 * workers.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_IOCP_WORKERS_H_
#define CUBE_SERVICE_TCP_IOCP_WORKERS_H_
#include <list>
#include <vector>

#include "cube/service/stdns.h"
#include "cube/service/tcp/handler.h"
#include "cube/service/tcp/iocp/worker.h"

BEGIN_SERVICE_TCP_NS
class workers{
public:
	workers();
	virtual ~workers();

	/**
	 * start workers by specified worker number
	 *@param num: worker number in workers
	 *@return:
	 *	0-success, otherwise for failed.
	 */
	int start(int num, void* arg = NULL);

	/**
	 * dispatch a handler to workers
	 */
	int dispatch(handler *hdr);

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
	std::vector<worker*> _workers;
};
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_IOCP_WORKERS_H_ */
