/*
 * session.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_CLIENT_EPOLL_SESSION_H_
#define CUBE_SERVICE_TCP_CLIENT_EPOLL_SESSION_H_
#include "cube/service/stdns.h"
#include "cube/service/stdsvc.h"
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
	int _sock;
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

	return 0;
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
#endif /* CUBE_SERVICE_TCP_CLIENT_EPOLL_SESSION_H_ */
