/*
 * connector.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_CLIENT_EPOLL_CLIENT_H_
#define CUBE_SERVICE_CLIENT_EPOLL_CLIENT_H_
#include <unistd.h>

#include "cube/service/stdns.h"
#include "cube/service/stdsvc.h"
#include "cube/service/tcp/client/client.h"
#include "cube/service/tcp/client/epoll/workers.h"

BEGIN_SERVICE_TCP_NS
using namespace std;
/*client service using epoll under linux*/
class eclient : public client{
public:
	eclient();
	virtual ~eclient();

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
	static void* client_thread_func(void *arg);

private:
	//connector of the client
	connector _connector;
	//workers of the client
	workers _workers;

	//thread identifier
	pthread_t _thread;
	//stop flag for client service
	bool _stop;
};

eclient::eclient(): _thread(0), _stop(true) {

}

eclient::~eclient() {
}

int eclient::start(int workers) {
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
	if(pthread_create(&_thread, 0, client_thread_func, this) != 0){
		_stop = true;
		return -1;
	}

	return 0;
}

int eclient::build(unsigned int ip, unsigned short port, handler *hdr){
	return _connector.connect(ip, port, hdr);
}

int eclient::stop() {
	/*check current client status*/
	if(_stop){
		return 0;
	}

	/*stop client thread*/
	_stop = true;
	pthread_join(_thread, 0);

	/*stop connector*/
	_connector.stop();

	/*stop workers*/
	_workers.stop();

	return 0;
}

void eclient::wait_for_next_loop(){
	/*wait for 5ms*/
	::usleep(5000);
}

void eclient::run_loop(){
	while(!_stop){
		/*wait a tiny time for next loop*/
		wait_for_next_loop();
	}
	pthread_exit(0);
}

void* eclient::client_thread_func(void* arg){
	eclient *pclient = (eclient*)arg;
	pclient->run_loop();
	return 0;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_CONNECTOR_H_ */
