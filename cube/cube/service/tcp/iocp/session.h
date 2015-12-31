/*
 * session.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_IOCP_SESSION_H_
#define CUBE_SERVICE_TCP_IOCP_SESSION_H_
#include <Winsock2.h>

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

session::session(): _sock(-1), _remote_ip(0), _remote_port(0){

}

session::~session(){
	if(_sock != -1){
		::close(_sock);
	}
}

int session::send(const void* data, unsigned int sz){
	DWORD send_sz = 0;
	olpdata *olp = new olpdata(data, sz);
	if(::WSASend(_sock, &olp->_data, 1, &send_sz, 0,&(olp->_overlapped), NULL) == SOCKET_ERROR){
		if(WSAGetLastError() != WSA_IO_PENDING) {
			delete olp;
			return -1;
		}
	}

	return (int)send_sz;
}

int session::recv(unsigned int sz){
	olpdata *olp = new olpdata(sz);
	if(::WSARecv(_sock, &(olp->_data), 1, NULL, NULL, &(olp->_overlapped), NULL) == SOCKET_ERROR) {
		if(WSAGetLastError() != WSA_IO_PENDING) {
			delete olp;
			return -1;
		}
	}

	return (int)sz;
}

bool session::redo_send(unsigned int *send_sz) {
	return false;
}

bool session::redo_recv(void** data, unsigned int *recv_sz){
	return false;
}

void session::sock(SOCKET s){
	_sock = s;
}

SOCKET session::sock() const{
	return _sock;
}

void session::remote_ip(unsigned int ip){
	_remote_ip = ip;
}

unsigned int session::remote_ip() const{
	return _remote_ip;
}

void session::remote_port(unsigned short port){
	_remote_port = port;
}

unsigned short session::remote_port() const{
	return _remote_port;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_IOCP_SESSION_H_ */
