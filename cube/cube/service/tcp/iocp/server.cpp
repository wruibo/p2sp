#include "cube/service/tcp/iocp/server.h"
BEGIN_SERVICE_TCP_NS
template<class handler>
server<handler>::server() : _sock(INVALID_SOCKET), _local_ip(0), _local_port(0), _arg(0), _thread(INVALID_HANDLE_VALUE), _thread_id(0), _stop(true) {

}

template<class handler>
server<handler>::~server() {

}

template<class handler>
int server<handler>::start(unsigned short port, int workers, void *arg /* = NULL */) {
	return this->start(INADDR_ANY, port, workers, arg);
}

template<class handler>
int server<handler>::start(unsigned int ip, unsigned short port, int workers, void *arg /* = NULL*/) {
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
	_thread = (HANDLE)::_beginthreadex(NULL, 0, accept_thread_func, this, 0, &_thread_id);
	if(_thread == NULL){
		_stop = true;
		return -1; //start accepter thread failed.
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
	::WaitForSingleObject(_thread, INFINITE);
	::CloseHandle(_thread);

	/*stop workers*/
	_workers.stop();


	/*close socket*/
	::closesocket(_sock);

	return 0;
}

template<class handler>
int server<handler>::run_accept_loop() {
	while (!_stop) {
		struct sockaddr_in remote;
		int addr_len = sizeof(remote);
		memset(&remote, 0, addr_len);
		SOCKET sock = ::WSAAccept(_sock, (struct sockaddr*)&remote, &addr_len, 0, 0);
		if (sock >= 0) { //new connection
			if (set_nonblock(sock) != 0) {
				::closesocket(sock);
				continue;
			}

			handler *hdr = new handler();
			hdr->sock(sock);
			hdr->remote_ip(ntohl(remote.sin_addr.s_addr));
			hdr->remote_port(ntohs(remote.sin_port));

			if (_workers.dispatch(hdr) != 0){
				delete hdr;
			}
		} else {
			; //print error, should nerve happened
		}
	}
	return 0;
}

template<class handler>
unsigned server<handler>::accept_thread_func(void *arg) {
	server<handler> *paccepter = (server<handler> *) arg;
	paccepter->run_accept_loop();
	::_endthreadex(0);
	return 0;
}
END_SERVICE_TCP_NS

