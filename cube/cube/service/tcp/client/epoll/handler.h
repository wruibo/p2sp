/*
 * handler.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_EPOLL_HANDLER_H_
#define CUBE_SERVICE_EPOLL_HANDLER_H_
#include <time.h>
#include "cube/service/stdns.h"
#include "cube/service/stdsvc.h"
BEGIN_SERVICE_TCP_NS
/*abstract handler for client using epoll connector*/
class handler
{
public:
	handler();
	virtual ~handler(void);

	/*
	 *	recall method after the connection has build
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_open();

	/*
	 *	recall method when the epoll write edge triggered, send data
	 *until can not send.
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_send();

	/*
	 *	recall method when the epoll read edge triggered, receive data
	 *until no data could be received.
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_recv();

	/*
	 *	recall method when error happened on the socket
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_error(int err);

	/*
	 *	recall method before socket closed, always invoked before the
	 *handler destroyed.
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_close();

	/*
	 *	recall method when peer shutdown the send channel. which means
	 *fin packet received
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_shutd();

	/*
	 *	recall method when timer triggered. timer event is set
	 *by calling the @set_timeout method
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_timeout();

	/*
	 *	recall method every tiny period
	 *return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int handle_run(time_t tm);

public:
	/*set & get the connection information*/
	void sock(int sock);
	int sock() const;
	void remote_ip(unsigned int ip);
	unsigned int remote_ip() const;
	void remote_port(unsigned short port);
	unsigned short remote_port() const;

	/*set the timer delay time in seconds*/
	void set_timeout(int delay_secs);

	/*check is the timer should be triggered*/
	bool is_timeout(time_t now);
protected:
	/*send data to peer*/
	ssize_t send(const void*buffer, size_t length, int flags);
	/*received data from peer*/
	ssize_t recv(char *buffer, int length, int flags);

private:
	//socket of the relate handler
	int _sock;
	//remote peer ip
	unsigned int _remote_ip;
	//remote peer port
	unsigned short _remote_port;

	//timer trigger timestamp
	time_t _timer_tm;
};

handler::handler(): _sock(-1), _remote_ip(0), _remote_port(0), _timer_tm(-1) {
}

handler::~handler() {
}

void handler::sock(int sock) {
	_sock = sock;
}

int handler::sock() const {
	return _sock;
}

void handler::remote_ip(unsigned int ip) {
	_remote_ip = ip;
}

unsigned int handler::remote_ip() const {
	return _remote_ip;
}

void handler::remote_port(unsigned short port) {
	_remote_port = port;
}

unsigned short handler::remote_port() const {
	return _remote_port;
}
void handler::set_timeout(int delay_secs) {
	_timer_tm = time(0)+delay_secs;
}

bool handler::is_timeout(time_t now)
{
	if(_timer_tm == ((time_t)-1) || _timer_tm > now){ //timer not set or not triggered
		return false;
	} else {//timer triggered, reset the timer
		_timer_tm = (time_t)(-1);
		return true;
	}
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_HANDLER_H_ */
