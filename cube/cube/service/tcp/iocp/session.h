/*
 * session.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_IOCP_SESSION_H_
#define CUBE_SERVICE_TCP_IOCP_SESSION_H_
#include <WinSock2.h>
#include <windows.h>

#include "cube/service/stdns.h"
#include "cube/service/tcp/iocp/olpdata.h"
BEGIN_SERVICE_TCP_NS
class session{
public:
	session();
	virtual ~session();

	/**
	 *	send data to remote peer
	 *@param data: data to send
	 *@param sz: size of data in bytes
	 *@return
	 *	data size sent or <0 indicate an error
	 */
	int send(const void* data, unsigned int sz);

	/**
	 * request receiving action with specified size
	 */
	int recv(unsigned int sz);

	/**
	 * ignored for iocp, only used for epoll
	 */
	bool redo_send(unsigned int *send_sz);

	/**
	 * ignored for iocp, only used for epoll
	 */
	bool redo_recv(void** data, unsigned int *recv_sz);

public:
	/**
	 * set&get the socket of the session
	 */
	void sock(SOCKET s);
	SOCKET sock() const;

	/**
	 * set&get the remote peer ip
	 */
	void remote_ip(unsigned int ip);
	unsigned int remote_ip() const;

	/**
	 * set&get the remote peer port
	 */
	void remote_port(unsigned short port);
	unsigned short remote_port() const;

private:
	//socket of session
	SOCKET _sock;
	//remote peer ip
	unsigned int _remote_ip;
	//remote peer port
	unsigned short _remote_port;
};
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_IOCP_SESSION_H_ */
