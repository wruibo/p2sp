#ifndef CUBE_SERVICE_TCP_SESSION_H_
#define CUBE_SERVICE_TCP_SESSION_H_
#include "cube/service/util/type.h"

BEGIN_SERVICE_NS
class tcp_session{
public:
	tcp_session();
	virtual ~tcp_session();

	/**
	 *	send data to remote peer
	 *@param data: data to send
	 *@param sz: size of data in bytes
	 *@return
	 *	data size sent, same value equal @sz
	 */
	int send(const void* data, unsigned int sz);

	/**
	 * request receiving action with specified size
	 * @param sz: size of bytes want to receive
	 * @return:
	 * 	same value equal @sz
	 */
	int recv(unsigned int sz);

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

public:
	/**
	 * set&get the socket of the session
	 */
	void sock(socket_t s);
	socket_t sock() const;

	/**
	 * set&get the remote peer ip
	 */
	void remote_ip(unsigned int ip);
	unsigned int remote_ip() const;

	/**
	 * set&get the remote peer port
	 */
	void remote_port(unsigned short port);
	unsigned short remote_port() const;

private:
	//socket of session
	socket_t _sock;
	//remote peer ip
	unsigned int _remote_ip;
	//remote peer port
	unsigned short _remote_port;
};
END_SERVICE_NS
#endif /* CUBE_SERVICE_TCP_SESSION_H_ */
