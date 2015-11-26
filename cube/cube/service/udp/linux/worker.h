/*
 * worker.h
 *
 *  Created on: 2015年11月24日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_LINUX_WORKER_H_
#define CUBE_SERVICE_UDP_LINUX_WORKER_H_
#include <list>
#include <pthread.h>
#include "cube/service/util/queue.h"
#include "cube/service/udp/handler.h"

namespace cube {
namespace service {
using namespace std;
/*
 *	worker class to process the udp packet received by the receiver thread
 */
template<class handler>
class worker {
public:
	worker(void);
	virtual ~worker(void);

	/*
	 *	start udp worker with local bind udp @socket, @arg what will be passed to the udp
	 *handler object
	 *return:
	 *	0--success, other--failed.
	 */
	int start(int socket, void* arg = 0);

	/*
	 *	stop udp worker
	 *return:
	 *	0--success, other--failed.
	 */
	int stop();

	/*
	 *	give a udp packet received by receiver to worker
	 *return:
	 *	0--success, other--failed.
	 */
	int process(dpacket_t *pkt);

private:
	/*udp process worker thread*/
	static void* worker_thread(void *arg);

private:
	//socket used to response
	int _socket;
	//arg to pass the handler call-back function
	void *_arg;

	//queue for received packet packet
	queue<dpacket_t*> _recvq;

	//udp handler
	handler *_handler;

	//worker thread handle & id
	pthread_t _thread;
	bool _stop;
};

template<class handler>
worker<handler>::worker() :
		_socket(-1), _arg(0), _handler(0), _thread(0), _stop(true) {

}

template<class handler>
worker<handler>::~worker() {

}

template<class handler>
int worker<handler>::start(int socket, void* arg) {
	/*initialize value*/
	_socket = socket;
	_arg = arg;
	_handler = new handler();

	/*initialize recv&send queue*/
	int err = _recvq.init(16*1024);
	if (err != 0)
		return -1;

	/*start the worker thread*/
	_stop = false;
	if (pthread_create(&_thread, 0, worker_thread, this) != 0) {
		_stop = true;
		return -1;
	}

	return 0;
}

template<class handler>
int worker<handler>::process(dpacket_t *pkt) {
	_recvq.write(pkt);
	return 0;
}

template<class handler>
int worker<handler>::stop() {
	/*stop thread*/
	if(!_stop){
		_stop = true;
		pthread_join(_thread, 0);
	}

	return 0;
}

template<class handler>
void* worker<handler>::worker_thread(void *arg) {
	worker *pworker = (worker*) arg;
	while (!pworker->_stop) {
		dpacket_t *req = 0;
		pworker->_recvq.read(&req);
		if (req != 0) {
			/*handle the request packet*/
			dpacket_t *resp = 0;
			int err = pworker->_handler->handle(req, &resp, pworker->_arg);
			if(err != 0 && resp != 0){
				/*send response to source*/
				::sendto(pworker->_socket, resp->_buf, resp->_dsz, 0, &req->_addr._addr, sizeof(req->_addr));
				/*free the response packet*/
				delete resp;
			}

			/*free the request packet*/
			delete req;
		}
	}

	pthread_exit(0);
	return 0;
}
}
}
#endif /* CUBE_SERVICE_UDP_LINUX_WORKER_H_ */
