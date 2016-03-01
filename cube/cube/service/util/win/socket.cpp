#include <WinSock2.h>
#include "cube/service/util/win/socket.h"

BEGIN_SERVICE_NS
int set_nonblock(socket_t sock) {
	unsigned long io_mode = 1;
	int err = ::ioctlsocket(sock, FIONBIO, &io_mode);
	if(err != 0){
		return -1;
	}

	return 0;
}

int set_reuse_addr(socket_t sock) {
	int on = 1;
	if(::setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const char*)&on,sizeof(on)) != 0)
		return -1;
	return 0;
}

socket_t tcp_create(unsigned int ip, unsigned short port){
	/*create socket*/
	socket_t sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if(sock == INVALID_SOCKET) {
		return -1; //create socket failed
	}

	/*set reuse address*/
	if(set_reuse_addr(sock) != 0){
		return -1;
	}

	/*bind socket to listen port*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ::htonl(ip);
	addr.sin_port = ::htons(port);
	int err = ::bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if(err == SOCKET_ERROR){
		return -1; //bind socket failed.
	}

	return sock;
}

/**
 * build connection to remote peer with @ip and @port
 * @return
 * 	socket for the connection, or <0 for failure
 */
socket_t async_tcp_connect(unsigned int ip, unsigned short port){
	/*create socket*/
	socket_t sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if(sock == INVALID_SOCKET)
		return INVALID_SOCKET; //create socket failed

	/*set socket to nonblock mode*/
	if(set_nonblock(sock) != 0){
		::closesocket(sock);
		return INVALID_SOCKET;
	}

	/*bind remote address*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);

	/*connect to remote*/
	if(::WSAConnect(sock, (struct sockaddr*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) != 0)
	{
		if(::WSAGetLastError() != WSAEWOULDBLOCK)
		{
			closesocket(sock);
			return INVALID_SOCKET;
		}
	}
	return sock;
}

socket_t udp_create(unsigned int ip, unsigned short port) {
	/*create socket*/
	socket_t sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		return INVALID_SOCKET;

	/*bind to local ip & port*/
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	int err = ::bind(sock, (SOCKADDR*) &addr, sizeof(addr));
	if (err == SOCKET_ERROR) {
		closesocket(sock);
		return INVALID_SOCKET; //bind to socket error
	}

	return sock;
}

/**
 *	create an udp socket for an remote peer with @ip and @port
 *	@param ip: remote peer ip
 *	@param port: remote peer port
 *	@return:
 *		socket on success, -1 when failed
 */
socket_t udp_connect(unsigned int ip, unsigned short port){
	/*create socket for remote peer*/
	socket_t sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		return sock;
	}

	/*connect to the remote peer*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	int err = ::connect(sock, (struct sockaddr*) &addr, sizeof(addr));
	if(err != 0){
		::closesocket(sock);
		return -1;
	}

	return sock;
}
END_SERVICE_NS



