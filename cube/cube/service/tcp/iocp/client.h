/*
 * connector.h
 */

#ifndef CUBE_SERVICE_CLIENT_IOCP_CLIENT_H_
#define CUBE_SERVICE_CLIENT_IOCP_CLIENT_H_
#include <process.h>
#include <Windows.h>

#include "cube/service/stdns.h"
#include "cube/service/stdsvc.h"
#include "cube/service/tcp/iocp/workers.h"

BEGIN_SERVICE_TCP_NS
using namespace std;
/*client service using io complete port under windows*/
class client{
public:
	client();
	virtual ~client();

	/**
	 * start the client service with specified workers
	 *@param workers: worker number for the service
	 *@return:
	 *	0-success, otherwise failed.
	 */
	virtual int start(int workers);

	/**
	 * build a connection to remote peer
	 *@param ip: remote peer id
	 *@param port: remote peer port
	 *@param hdr: handler for process the connection
	 *@return:
	 *	0-success, otherwise failed.
	 */
	virtual int build(unsigned int ip, unsigned short port, handler *hdr);

	/**
	 * stop the client service
	 *@return:
	 *	0-success, otherwise failed.
	 */
	virtual int stop();

private:
	/**
	 * pull established handlers from connector, dispatch to workers
	 */
	void process_established_handlers();

	/**
	 * wait a tiny time for next loop in client thread
	 */
	void wait_for_next_loop();

	/**
	 * loop for client thread
	 */
	void run_loop();

	/**
	 * thread function for client thread
	 */
	static unsigned * client_thread_func(void *arg);

private:
	//connector of the client
	connector _connector;
	//workers of the client
	workers _workers;

	//thread handle
	HANDLE _thread;
	unsigned int _thread_id;
	//stop flag for client service
	bool _stop;
};

client::client(): _thread(NULL), _thread_id(0), _stop(true) {

}

client::~client() {
}

int client::start(int workers) {
	/*check if client has been started*/
	if(!_stop){
		return 0;
	}

	/*start workers*/
	if(_workers.start(workers) != 0){
		return -1;;
	}

	/*start connector*/
	if(_connector.start(&_workers) != 0){
		return -1;
	}

	/*start client thread*/
	_stop = false;
	_thread = (HANDLE)::_beginthreadex(NULL, 0, client_thread_func, this, 0, &_thread_id);
	if(_thread == NULL){
		_stop = true;
		return -1;
	}

	return 0;
}

int client::build(unsigned int ip, unsigned short port, handler *hdr){
	return _connector.connect(ip, port, hdr);
}

int client::stop() {
	/*check current client status*/
	if(_stop){
		return 0;
	}

	/*stop client thread*/
	_stop = true;
	::WaitForSingleObject(_thread, INFINITE);
	CloseHandle(_thread);

	/*stop connector*/
	_connector.stop();

	/*stop workers*/
	_workers.stop();

	return 0;
}

void client::wait_for_next_loop(){
	/*wait for 5ms*/
	::Sleep(5);
}

void client::run_loop(){
	while(!_stop){
		/*wait a tiny time for next loop*/
		wait_for_next_loop();
	}
	::_endthreadex(0);
}

unsigned* client::client_thread_func(void* arg){
	client *pclient = (client*)arg;
	pclient->run_loop();
	return 0;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_CONNECTOR_H_ */
