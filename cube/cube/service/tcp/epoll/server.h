#ifndef CUBE_SERVICE_TCP_EPOLL_SERVER_H_
#define CUBE_SERVICE_TCP_EPOLL_SERVER_H_
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <cube/service/tcp/epoll/workers.h>
#include <sys/socket.h>

#include "cube/service/stdns.h"
#include "cube/service/util/socket.h"

BEGIN_SERVICE_TCP_NS
/**
 * epoll server service using EDGE trigger
 */
template<class handler>
class server {
public:
	server();
	virtual ~server();

	/**
	 *	start tcp server with specified listening port and workers number, passing
	 *the @arg to on open method
	 *@param port: listen port for tcp server
	 *@param workers: number of workers
	 *@param arg: argument will be passed to open method
	 *@return:
	 *	0--sucess, otherwise for failed
	 */
	int start(unsigned short port, int workers, void *arg = 0);

	/**
	 * start tcp server with specified binding ip, listening port, and workers number,
	 * passing the @arg to open method
	 *@param ip: binding local ip
	 *@param port: listen port for tcp server
	 *@param workers: number of workers
	 *@param arg: argument will be passed to open method
	 *@return:
	 *	0--sucess, otherwise for failed
	 */
	int start(unsigned int ip, unsigned short port, int workers, void *arg = 0);

	/**
	 * stop the tcp server
	 */
	int stop();

private:
	/**
	 * accept loop
	 */
	int run_accept_loop();

	/**
	 * thread function for accept socket from remote peer
	 */
	static void * accept_thread_func(void *arg);

private:
	//listening socket
	int _sock;
	//local binding ip
	unsigned int _local_ip;
	//local listening port
	unsigned short _local_port;
	//argument passing to the handler
	void *_arg;

	//workers of the client
	workers _workers;

	//thread identifier
	pthread_t _thread;
	//stop flag for client service
	bool _stop;
};

template<class handler>
server<handler>::server() : _sock(-1), _local_ip(0), _local_port(0), _arg(0), _thread(0), _stop(true) {

}

template<class handler>
server<handler>::~server() {

}

template<class handler>
int server<handler>::start(unsigned short port, int workers, void *arg /* = 0 */) {
	return this->start(INADDR_ANY, port, workers, arg);
}

template<class handler>
int server<handler>::start(unsigned int ip, unsigned short port, int workers, void *arg /* = 0*/) {
	if (workers <= 0) {
		return -1;
	}

	/*initial member*/
	_local_ip = ip;
	_local_port = port;
	_arg = arg;

	/*start workers*/
	if(_workers.start(workers, arg) != 0){
		return -1;;
	}

	/*create the listen socket*/
	_sock = tcp_create(ip, port);
	if(_sock < 0){
		return -1;
	}

	/*start listening*/
	if (::listen(_sock, SOMAXCONN) != 0){
		return -1;
	}

	/*start the accept thread*/
	_stop = false;
	if (pthread_create(&_thread, 0, accept_thread_func, this) != 0) {
		_stop = true;
		return -1;
	}

	return 0;
}

template<class handler>
int server<handler>::stop() {
	if(_stop){
		return 0;
	}

	/*stop accept thread first*/
	_stop = true;
	pthread_join(_thread, 0);

	/*stop workers*/
	_workers.stop();


	/*close socket*/
	::close(_sock);

	return 0;
}

template<class handler>
int server<handler>::run_accept_loop() {
	while (!_stop) {
		struct sockaddr_in remote_addr;
		socklen_t addr_len = sizeof(remote_addr);
		memset(&remote_addr, 0, addr_len);
		int sock = ::accept(_sock, (struct sockaddr*) &remote_addr, &addr_len);
		if (sock >= 0) { //new connection
			if (set_nonblock(sock) != 0) {
				close(sock);
				continue;
			}

			handler *hdr = new handler();
			hdr->sock(sock);
			hdr->remote_ip(ntohl(remote_addr.sin_addr.s_addr));
			hdr->remote_port(ntohs(remote_addr.sin_port));

			if (_workers.dispatch(hdr) != 0){
				delete hdr;
			}
		} else {
			; //print error, should nerve happened
		}
	}
}

template<class handler>
void* server<handler>::accept_thread_func(void *arg) {
	server<handler> *paccepter = (server<handler> *) arg;
	paccepter->run_accept_loop();
	pthread_exit(0);
	return 0;
}
END_SERVICE_TCP_NS
#endif
