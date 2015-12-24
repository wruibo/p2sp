/*
 * session.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_CLIENT_EPOLL_SESSION_H_
#define CUBE_SERVICE_TCP_CLIENT_EPOLL_SESSION_H_
#include "cube/service/stdns.h"
BEGIN_SERVICE_TCP_NS
class session{
public:
	session();
	virtual ~session();

	/**
	 * initialize the session data
	 *@param sock: tcp socket for the session
	 *@param ip: remote peer ip
	 *@param port:remote peer port
	 */
	void init(int sock, unsigned int ip, unsigned short port);

	/**
	 *	send data to remote peer
	 *@param data: data to send
	 *@param sz: size of data in bytes
	 *@return
	 *	data size sent or <0 indicate an error
	 */
	int send(const void* data, unsigned int sz);


	/**
	 * get the socket of the session
	 */
	int sock() const;

	/**
	 * get the remote peer ip
	 */
	unsigned int remote_ip() const;

	/**
	 * get the remote peer port
	 */
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

void session::init(int sock, unsigned int ip, unsigned short port){
	_sock = sock;
	_remote_ip = ip;
	_remote_port = port;
}

int session::send(const void* data, unsigned int sz){
	return 0;
}

int session::sock() const{
	return _sock;
}

unsigned int session::remote_ip() const{
	return _remote_ip;
}

unsigned short session::remote_port() const{
	return _remote_port;
}

END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_CLIENT_EPOLL_SESSION_H_ */
