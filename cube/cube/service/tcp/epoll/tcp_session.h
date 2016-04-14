/*
 * session.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_EPOLL_TCP_SESSION_H_
#define CUBE_SERVICE_TCP_EPOLL_TCP_SESSION_H_
#include <list>
#include "cube/service/stdns.h"
BEGIN_SERVICE_TCP_NS
class datablk {
public:
	datablk(const void* data, unsigned int sz){
		_sz = sz;
		_data = new char[sz];
		memcpy(_data, data, sz);
		_pos = 0;
	}

	virtual ~datablk(){
		delete []_data;
		_sz = 0;
		_pos = 0;
	}

public:
	void *_data;
	unsigned int _sz;
	unsigned int _pos;
};

class session{
public:
	session();
	virtual ~session();

	/**
	 *	send data to remote peer
	 *@param data: data to send
	 *@param sz: size of data in bytes
	 *@return
	 *	data size sent, same value equal @sz
	 */
	int send(const void* data, unsigned int sz);

	/**
	 * request receiving action with specified size
	 * @param sz: size of bytes want to receive
	 * @return:
	 * 	same value equal @sz
	 */
	int recv(unsigned int sz);

	/**
	 * process the data sending job
	 * @param send_sz: data size has been sent
	 * @return:
	 * 	true-it need to continue sending, false-send job completed
	 */
	bool redo_send(unsigned int *send_sz);

	/**
	 * process the data receiving job
	 * @param data: data received, need to be released after use outside
	 * @param recv_sz: data size received
	 * @return:
	 * true-it need to be continue receiving, false-receive job completed
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
	int _sock;
	//remote peer ip
	unsigned int _remote_ip;
	//remote peer port
	unsigned short _remote_port;

	//data waiting for send
	std::list<datablk*> _sendblks;

	//data size want to receive
	unsigned int _recvsz;
};

session::session(): _sock(-1), _remote_ip(0), _remote_port(0), _recvsz(0){

}

session::~session(){
	if(_sock != -1){
		::close(_sock);
	}
}

int session::send(const void* data, unsigned int sz){
	datablk* blk = new datablk(data, sz);
	_sendblks.push_back(blk);
	return sz;
}

int session::recv(unsigned int sz) {
	_recvsz = sz;
	return sz;
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
#endif /* CUBE_SERVICE_TCP_EPOLL_TCP_SESSION_H_ */
