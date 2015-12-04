/*
 * socket.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_WIN_SOCKET_H_
#define CUBE_SERVICE_UTIL_WIN_SOCKET_H_
#include "cube/service/netstd.h"

namespace cube {
namespace service {

static SOCKET udp_create(unsigned int ip, unsigned short port) {
	/*create socket*/
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		return INVALID_SOCKET;

	/*bind to local ip & port*/
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	int err = ::bind(sock, (SOCKADDR*) &addr, sizeof(addr));
	if (err == SOCKET_ERROR) {
		closesocket(sock);
		return INVALID_SOCKET; //bind to socket error
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
static SOCKET udp_connect(unsigned int ip, unsigned short port){
	/*create socket for remote peer*/
	SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, 0);
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
		::closesocket(sock);
		return -1;
	}

	return sock;
}

}
}

#endif /* CUBE_SERVICE_UTIL_WIN_SOCKET_H_ */
