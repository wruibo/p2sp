/*
 * client.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_CLIENT_LINUX_CLIENT_H_
#define CUBE_SERVICE_UDP_CLIENT_LINUX_CLIENT_H_
#include "cube/service/udp/client/linux/worker.h"


namespace cube {
namespace service {
/**
 * udp client service
 */
class client {
public:
	client() {
	}
	virtual ~client() {
	}

	/**
	 * start the udp client service with specified worker number @worknum
	 * @param worknum: worker numbers
	 * @return:
	 *	0-start client service success, otherwise failed.
	 */
	int start(int worknum);

	/**
	 * start the udp client service with local bind udp port, which used for
	 * interact with remote peers
	 * @param port: local bind udp port
	 * @param worknum: worker numbers
	 * @return:
	 * 	0-start client service success, otherwise failed.
	 */
	int start(unsigned short port, int worknum);

	/**
	 *	open an udp interaction with a remote peer
	 * @param ip: remote peer ip
	 * @param port: remote peer port
	 * @param handler: handler for processing the interaction
	 * @return:
	 * 	0-success, otherwise failed
	 */
	int open(unsigned int ip, unsigned short port, handler *hd);

	/**
	 * stop the client service
	 */
	int stop();

private:
	/**
	 * packet receive thread for local port binding udp client service
	 */
	static void* recv_thread(void* arg);

private:
	//socket for binding to the local udp port
	int _socket;
	unsigned short _port;

	//receive thread handle
	pthread_t _thread;
	//stop flag for the receive thread
	bool _stop;
};

int client::start(int worknum){
	return 0;
}

int client::start(unsigned short port, int worknum){
	/*bind socket with local specified port*/
	_port = port;
	_socket = socket(PF_INET, SOCK_DGRAM, 0);

	return 0;
}

int client::open(unsigned int ip, unsigned short port, handler *hd){
	return 0;
}

int client::stop(){
	return 0;
}

void* client::recv_thread(void* arg){
	return 0;
}

}
}

#endif /* CUBE_SERVICE_UDP_CLIENT_LINUX_CLIENT_H_ */
