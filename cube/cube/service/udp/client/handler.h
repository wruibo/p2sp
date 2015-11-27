/*
 * chandler.h
 *
 *  Created on: 2015年11月26日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_CLIENT_HANDLER_H_
#define CUBE_SERVICE_UDP_CLIENT_HANDLER_H_

namespace cube{
namespace service{
class handler{
public:
	handler();
	virtual ~handler();

	virtual int on_open(void *arg);
	virtual int on_send();
	virtual int on_recv();
	virtual int on_close();
	virtual int on_error();
	virtual int on_timeout();

protected:
	//argument passed from the @open function in @client class object
	void* arg;

	unsigned int _local_ip;
	unsigned int _local_port;
	unsigned int _remote_ip;
	unsigned short _remote_port;
};
}
}

#endif /* CUBE_SERVICE_UDP_CLIENT_HANDLER_H_ */
