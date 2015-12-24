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
	friend class worker;
	friend class connector;

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

protected:
	timer&  timer();
	session& session();

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

timer& handler::timer(){
	return _timer;
}

session& handler::session(){
	return _session;
}

END_SERVICE_TCP_NS

#endif /* CUBE_SERVICE_TCP_CLIENT_HANDLER_H_ */
