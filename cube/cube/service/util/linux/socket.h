/*
 * socket.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_LINUX_SOCKET_H_
#define CUBE_SERVICE_UTIL_LINUX_SOCKET_H_
#include <string.h>
#include "cube/service/netstd.h"

namespace cube {
namespace service {
/**
 * create udp socket, and bind the socket to the specified @ip and @port,
 * return the socket if create success, otherwise return -1.
 */
static int udp_create(unsigned int ip, unsigned short port) {
	/*create socket*/
	int sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		return -1; //create socket failed

	/*bind to port*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	int err = ::bind(sock, (struct sockaddr*) &addr, sizeof(addr));
	if (err != 0){
		::close(sock);
		return -1; //bind to socket error
	}

	return sock;
}

/**
 *	create an udp socket for an remote peer with @ip and @port
 *	@param ip: remote peer ip
 *	@param port: remote peer port
 *	@return:
 *		socket on success, -1 when failed
 */
static int udp_connect(unsigned int ip, unsigned short port){
	/*create socket for remote peer*/
	int sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		return sock;
	}

	/*connect to the remote peer*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	int err = ::connect(sock, (struct sockaddr*) &addr, sizeof(addr));
	if(err != 0){
		::close(sock);
		return -1;
	}

	return sock;
}
}
}

#endif /* CUBE_SERVICE_UTIL_LINUX_SOCKET_H_ */
