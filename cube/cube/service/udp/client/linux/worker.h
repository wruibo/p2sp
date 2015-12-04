/*
 * worker.h
 *
 *  Created on: 2015年11月26日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_CLIENT_LINUX_WORKER_H_
#define CUBE_SERVICE_UDP_CLIENT_LINUX_WORKER_H_
#include "cube/service/udp/client/handler.h"

namespace cube{
namespace service{
class worker{
public:
	worker(){

	}
	virtual ~worker(){

	}

	int start();

	int stop();

private:
	static void* work_thread(void* arg);
};
}
}

#endif /* CUBE_SERVICE_UDP_CLIENT_LINUX_WORKER_H_ */
