/*
 * connector.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_CLIENT_CLIENT_H_
#define CUBE_SERVICE_CLIENT_CLIENT_H_
#include "cube/service/stdns.h"
#include "cube/service/tcp/client/handler.h"

BEGIN_SERVICE_TCP_NS
using namespace std;
/**
 * client service for tcp base application
 */
class client {
public:
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
	 *@param hdr: handler for process the connection, !!must be new outside, will be released in the client!!
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

public:
	/**
	 * create a new instance of the client service
	 *@return:
	 *	new instance of client service
	 */
	static client* new_instance();

	/**
	 * return the exist singleton instance of the client service
	 *@return:
	 *	singleton instance of client service
	 */
	static client* get_instance();

	virtual ~client();

private:
	client();

private:
	//singleton instance of client service
	static client* _instance;
};

/*initialize the singleton instance of client*/
static client* client::_instance = 0;

client::client(){

}

client::~client(){

}

client* client::new_instance(){
	return 0;
}

client* client::get_instance(){
	if(_instance != 0){
		return _instance;
	}
	return 0;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_EPOLL_CONNECTOR_H_ */
