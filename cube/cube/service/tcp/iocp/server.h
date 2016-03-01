#ifndef CUBE_SERVICE_TCP_IOCP_SERVER_H_
#define CUBE_SERVICE_TCP_IOCP_SERVER_H_
#include <WinSock2.h>
#include <process.h>

#include "cube/service/stdns.h"
#include "cube/service/util/socket.h"
#include "cube/service/tcp/iocp/workers.h"

BEGIN_SERVICE_TCP_NS
/**
 * iocp server service
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
	static unsigned __stdcall accept_thread_func(void *arg);

private:
	//listening socket
	SOCKET _sock;
	//local binding ip
	unsigned int _local_ip;
	//local listening port
	unsigned short _local_port;
	//argument passing to the handler
	void *_arg;

	//workers of the client
	workers _workers;

	//thread handle
	HANDLE _thread;
	unsigned int _thread_id;
	//stop flag for client service
	bool _stop;
};
END_SERVICE_TCP_NS
#endif
