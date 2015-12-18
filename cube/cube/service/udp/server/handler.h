/*
 * handler.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_SERVER_HANDLER_H_
#define CUBE_SERVICE_UDP_SERVER_HANDLER_H_
#include "../../stdsvc.h"

namespace cube {
namespace service {

/*network address*/
class addr_t {
public:
	unsigned int ip() {
		return ::ntohl(((struct sockaddr_in*) &_addr)->sin_addr.s_addr);
	}
	unsigned short port() {
		return ::ntohs(((struct sockaddr_in*) &_addr)->sin_port);
	}

	/*ipv4 address for socket*/
	struct sockaddr _addr;
};

/*udp datagram packet class*/
class dpacket_t {
public:
	dpacket_t(int bsz) :
			_bsz(bsz), _dsz(0) {
		_buf = new char[bsz];
	}

	virtual ~dpacket_t() {
		delete[] _buf;
		_buf = 0;
	}

	//packet source address
	addr_t _addr;
	//data buffer
	char *_buf;
	//buffer size
	int _bsz;
	//data size
	int _dsz;
};

/*
 *	base class of the udp service handler, must be realized when
 *using the udp service module.
 */
class handler {
public:
	handler(void) {
	}
	virtual ~handler(void) {
	}

public:
	/*
	 *	handle udp request with request package object @req, you can send response
	 *by using the @sendto method, @arg is the input the the udp service when startup.
	 *return:
	 *	0--handle success, <0--something error happened.
	 */
	virtual int handle(const dpacket_t *req, dpacket_t **resp, void *arg) = 0;
};
}
}
#endif /* CUBE_SERVICE_UDP_SERVER_HANDLER_H_ */
