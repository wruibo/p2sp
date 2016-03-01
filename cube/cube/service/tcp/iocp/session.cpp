#include "cube/service/tcp/iocp/session.h"
BEGIN_SERVICE_TCP_NS
session::session(): _sock(INVALID_SOCKET), _remote_ip(0), _remote_port(0){

}

session::~session(){
	if(_sock != INVALID_SOCKET){
		::closesocket(_sock);
	}
}

int session::send(const void* data, unsigned int sz){
	DWORD send_sz = 0;
	olpdata *olp = new olpdata(data, sz);
	if(::WSASend(_sock, &olp->_data, 1, &send_sz, 0,&(olp->_overlapped), NULL) == SOCKET_ERROR){
		if(WSAGetLastError() != WSA_IO_PENDING) {
			delete olp;
			return -1;
		}
	}

	return (int)send_sz;
}

int session::recv(unsigned int sz){
	olpdata *olp = new olpdata(sz);
	if(::WSARecv(_sock, &(olp->_data), 1, NULL, NULL, &(olp->_overlapped), NULL) == SOCKET_ERROR) {
		if(WSAGetLastError() != WSA_IO_PENDING) {
			delete olp;
			return -1;
		}
	}

	return (int)sz;
}

bool session::redo_send(unsigned int *send_sz) {
	return false;
}

bool session::redo_recv(void** data, unsigned int *recv_sz){
	return false;
}

void session::sock(SOCKET s){
	_sock = s;
}

SOCKET session::sock() const{
	return _sock;
}

void session::remote_ip(unsigned int ip){
	_remote_ip = ip;
}

unsigned int session::remote_ip() const{
	return _remote_ip;
}

void session::remote_port(unsigned short port){
	_remote_port = port;
}

unsigned short session::remote_port() const{
	return _remote_port;
}
END_SERVICE_TCP_NS
