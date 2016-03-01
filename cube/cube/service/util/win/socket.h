/*
 * socket.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_WIN_SOCKET_H_
#define CUBE_SERVICE_UTIL_WIN_SOCKET_H_
#include "cube/service/stdns.h"

BEGIN_SERVICE_NS
typedef unsigned int socket_t;

extern int set_nonblock(socket_t sock);

extern int set_reuse_addr(socket_t sock);

extern socket_t tcp_create(unsigned int ip, unsigned short port);

/**
 * build connection to remote peer with @ip and @port
 * @return
 * 	socket for the connection, or <0 for failure
 */
extern socket_t async_tcp_connect(unsigned int ip, unsigned short port);

extern socket_t udp_create(unsigned int ip, unsigned short port);
/**
 *	create an udp socket for an remote peer with @ip and @port
 *	@param ip: remote peer ip
 *	@param port: remote peer port
 *	@return:
 *		socket on success, -1 when failed
 */
extern socket_t udp_connect(unsigned int ip, unsigned short port);
END_SERVICE_NS
#endif /* CUBE_SERVICE_UTIL_WIN_SOCKET_H_ */
