/*
 * socket.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_WIN_SOCKET_H_
#define CUBE_SERVICE_UTIL_WIN_SOCKET_H_
#include "cube/service/stdsvc.h"

namespace cube {
namespace service {

static int set_nonblock(SOCKET sock) {
	unsigned long io_mode = 1;
	int err = ::ioctlsocket(sock, FIONBIO, &io_mode);
	if(err != 0){
		return -1;
	}

	return 0;
}

/**
 * build connection to remote peer with @ip and @port
 * @return
 * 	socket for the connection, or <0 for failure
 */
static SOCKET async_tcp_connect(unsigned int ip, unsigned short port){
	/*create socket*/
	SOCKET sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if(sock == INVALID_SOCKET)
		return INVALID_SOCKET; //create socket failed

	/*set socket to nonblock mode*/
	if(set_nonblock(sock) != 0){
		::closesocket(sock);
		return INVALID_SOCKET;
	}

	/*bind remote address*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);

	/*connect to remote*/
	if(::WSAConnect(sock, (struct sockaddr*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) != 0)
	{
		if(::WSAGetLastError() != WSAEWOULDBLOCK)
		{
			closesocket(sock);
			return INVALID_SOCKET;
		}
	}
	return sock;
}

static SOCKET udp_create(unsigned int ip, unsigned short port) {
	/*create socket*/
	SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, 0);
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
