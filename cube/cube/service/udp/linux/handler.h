/*
 * handler.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_LINUX_HANDLER_H_
#define CUBE_SERVICE_UDP_LINUX_HANDLER_H_
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "cube/service/util/linux/queue.h"

namespace cube {
namespace service {

/*udp datagram packet class*/
class dpacket_t {
	friend class udp_receiver;
public:
	dpacket_t(int bsz) : _bsz(bsz), _dsz(0) {
		memset(&_addr, 0, sizeof(_addr));
		_buf = new char[bsz];
	}

	~dpacket_t() {
		delete[] _buf;
		_buf = 0;
	}

	/*set the data of the packet*/
	void set(const char *data, int sz) {
		int csz = sz > _bsz ? _bsz : sz;
		memcpy(_buf, data, csz);
		_dsz = csz;
	}

	/*set the peer address relate with the packet*/
	inline void addr(const struct sockaddr *a) {
		memcpy(&_addr, a, sizeof(struct sockaddr));
	}

	/*return the peer address relate with the packet*/
	inline const struct sockaddr* addr() const {
		return &_addr;
	}

	/*return the host address ip relate with the packet*/
	inline const unsigned int ip() const {
		return ntohl(((struct sockaddr_in*) &_addr)->sin_addr.s_addr);
	}

	/*return the host address port relate with the packet*/
	inline const unsigned short port() const {
		return ntohs(((struct sockaddr_in*) &_addr)->sin_port);
	}

	/*get the write or read pointer for the buffer of packet object*/
	inline char *wptr() {
		return _buf;
	}

	inline const char *rptr() const {
		return _buf;
	}

	/*get&set the buffer size or data size of the packet object*/
	inline const int bsz() const {
		return _bsz;
	}

	inline const int dsz() const {
		return _dsz;
	}

	inline void dsz(int sz) {
		_dsz = sz;
	}

private:
	/*peer information relate with the packet*/
	//peer address
	struct sockaddr _addr;

	//data buffer
	char *_buf;
	//size of buffer
	int _bsz;
	//length of data in buffer
	int _dsz;
};

/*
 *	base class of the udp service handler, must be realized when
 *using the udp service module.
 */
class handler {
public:
	handler(void);
	virtual ~handler(void);

	/*set the local service information*/
	inline void set_socket(int sock) {
		_sock = sock;
	}
	inline void set_ip(unsigned int ip) {
		_ip = ip;
	}
	inline void set_port(unsigned short port) {
		_port = port;
	}

	inline int get_socket() {
		return _sock;
	}
	inline unsigned int get_ip() {
		return _ip;
	}
	inline unsigned short get_port() {
		return _port;
	}

public:
	/*
	 *	handle udp request with request package object @req, you can send response
	 *by using the @sendto method, @arg is the input the the udp service when startup.
	 *return:
	 *	0--handle success, <0--something error happened.
	 */
	virtual int handle(dpacket_t *req, void *arg = 0) = 0;

protected:
	/*
	 *	send response packet to the peer. first make a @dpacket_t object for the
	 *response data @resp with size @sz, and push it to the send queue.
	 *return:
	 *	0--send success, <0--something error happened.
	 */
	int sendto(const struct sockaddr *to, const char *resp, int sz);

private:
	/*local udp service information*/
	//local socket
	int _sock;
	//local ip
	unsigned int _ip;
	//local port
	unsigned short _port;
};
}
}
#endif /* CUBE_SERVICE_UDP_LINUX_HANDLER_H_ */
