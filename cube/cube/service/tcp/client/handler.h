/*
 * handler.h
 *
 *  Created on: 2015年12月21日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_CLIENT_HANDLER_H_
#define CUBE_SERVICE_TCP_CLIENT_HANDLER_H_
#include "cube/service/stdns.h"
#include "cube/service/tcp/client/timer.h"
#include "cube/service/tcp/client/session.h"
BEGIN_SERVICE_TCP_NS
class handler{
public:
	/*
	 * called after the connection has been built successfully.
	 *@return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int on_open();

	/*
	 *	called after the data has been sent
	 *@param sz: size has been sent in bytes
	 *@return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int on_send(int sz);

	/*
	 *	called after the data has been received
	 *@param data: data has been received
	 *@param sz: data size in bytes
	 *@return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int on_recv(const void *data, unsigned int sz);

	/*
	 *	called when the timer has triggered.
	 *@param tm: current time stamp
	 *@return:
	 *	0--success, other--failed
	 */
	virtual int on_timeout(time_t tm);

	/*
	 *	called every tiny interval, make the handler to process something else.
	 *@param tm: current time stamp
	 *@return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int on_running(time_t tm);

	/*
	 *	called when the connection closed.
	 *@return:
	 *	0--success, other--failed
	 */
	virtual int on_close(int err);
public:
	handler();
	virtual ~handler(void);

	/**
	 * set&get the socket of session
	 */
	void sock(SOCKET s);
	SOCKET sock();

	/**
	 * set&get the remote ip of session
	 */
	void remote_ip(unsigned int ip);
	unsigned int remote_ip();

	/**
	 * set&get the remote port of session
	 */
	void remote_port(unsigned short port);
	unsigned short remote_port();

	/**
	 * set timer which will be triggered after specified delay seconds
	 */
	void set_timer(int delay_seconds);
	/**
	 * check if timer has been triggered
	 */
	bool is_timeout(time_t now);

public:
	/**
	 *	send data to remote peer
	 *@param data: data to send
	 *@param sz: size of data in bytes
	 *@return
	 *	data size sent or <0 indicate an error
	 */
	int send(const void* data, int sz);

	/**
	 * request receive data action from remote peer
	 */
	int recv();

public:
	/**
	 * process the data sending job
	 */
	void do_send();

	/**
	 * process the data receiving job
	 */
	void do_recv();

private:
	//timer for handler;
	timer _timer;

	//tcp session for the handler
	session _session;
};

handler::handler(){

}

handler::~handler(){

}

void handler::sock(SOCKET s){
	_session.sock(s);
}

SOCKET handler::sock(){
	return _session.sock();
}

void handler::remote_ip(unsigned int ip){
	_session.remote_ip(ip);
}

unsigned int handler::remote_ip(){
	return _session.remote_ip();
}

void handler::remote_port(unsigned short port){
	_session.remote_port(port);
}

unsigned short handler::remote_port(){
	return _session.remote_port();
}

void handler::set_timer(int delay_seconds){
	_timer.set(delay_seconds);
}

bool handler::is_timeout(time_t now){
	return _timer.is_timeout(now);
}

int handler::send(const void* data, int sz){
	return _session.send(data, sz);
}

int handler::recv(){
	return 0;
}

void handler::do_send(){

}

void handler::do_recv(){

}
END_SERVICE_TCP_NS

#endif /* CUBE_SERVICE_TCP_CLIENT_HANDLER_H_ */
