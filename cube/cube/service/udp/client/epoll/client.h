/*
 * client.h
 *
 *  Created on: 2015年12月1日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_CLIENT_EPOLL_CLIENT_H_
#define CUBE_SERVICE_UDP_CLIENT_EPOLL_CLIENT_H_
#include "cube/service/util/socket.h"
#include "cube/service/udp/client/epoll/worker.h"

namespace cube{
namespace service{
namespace udp{
/**
 * udp client service
 */
class client {
public:
	client();
	virtual ~client();

	/**
	 * start the udp client service with specified worker number @worknum
	 * @param worknum: worker numbers
	 * @return:
	 *	0-start client service success, otherwise failed.
	 */
	int start(int worknum);

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
	//worker number
	int _worknum;
	//worker position for next peer
	int _pos;
	//workers for the client
	std::vector<worker*> _workers;
};

client::client():
		_worknum(0), _pos(0){

}
client::~client(){

}

int client::start(int worknum){
	/*start the workers*/
	if(worknum < 1){
		return -1;
	}
	_worknum = worknum;

	for(int i=0; i<worknum; i++){
		worker* pworker = new worker();
		if(pworker->start() != 0){
			delete pworker;
			return -1;
		}
		_workers.push_back(pworker);
	}

	return 0;
}

int client::open(unsigned int ip, unsigned short port, handler *hd){
	/*create socket relate with remote peer*/
	int socket = udp_connect(ip, port);
	if(socket < 0){
		::close(socket);
		return -1;
	}

	/*add the new peer to a selected worker*/
	_workers[++_pos%_worknum]->add(socket, hd);

	return 0;
}

int client::stop(){
	int num = (int)_workers.size();
	for(int i=0; i<num; i++){
		_workers[i]->stop();
		delete _workers[i];
	}
	_workers.clear();

	return 0;
}

}
}
}
#endif /* CUBE_SERVICE_UDP_CLIENT_EPOLL_CLIENT_H_ */
