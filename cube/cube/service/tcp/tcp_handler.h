#ifndef CUBE_SERVICE_TCP_TCP_HANDLER_H_
#define CUBE_SERVICE_TCP_TCP_HANDLER_H_
#include "cube/service/stdns.h"
#include "cube/service/util/type.h"
#include "cube/service/util/timer.h"
#include "cube/service/tcp/tcp_session.h"

BEGIN_SERVICE_NS
class tcp_handler{
public:
	/*
	 * called after the connection has been built successfully.
	 *@return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int on_open(void *arg = 0);

	/*
	 *	called after the data has been sent
	 *@param sz: size has been sent in bytes
	 *@return:
	 *	0--success, other--failed, handler will be destroyed
	 */
	virtual int on_send(unsigned int sz);

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
	tcp_handler();
	virtual ~tcp_handler(void);

	/**
	 * set timer which will be triggered after specified delay seconds
	 */
	void set_timer(int delay_seconds);

	/**
	 * check if timer has been triggered
	 */
	bool is_timeout(time_t now);

protected:
	/**
	 *	send data to remote peer
	 *@param data: data to send
	 *@param sz: size of data in bytes
	 *@return
	 *	data size sent or <0 indicate an error
	 */
	int send(const void* data, int sz);

public:
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

private:
	//timer for handler;
	timer _timer;

	//tcp session for the handler
	tcp_session _session;
};
END_SERVICE_NS
#endif /* CUBE_SERVICE_TCP_TCP_HANDLER_H_ */
