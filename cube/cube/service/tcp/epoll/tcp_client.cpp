#include "cube/service/tcp/tcp_client.h"

BEGIN_SERVICE_NS
tcp_client::tcp_client(): _thread(0), _stop(true) {

}

tcp_client::~tcp_client() {
}

int tcp_client::start(int workers) {
	/*check if tcp_client has been started*/
	if(!_stop){
		return 0;
	}

	/*start workers*/
	if(_workers.start(workers) != 0){
		return -1;;
	}

	/*start connector*/
	if(_connector.start(&_workers) != 0){
		return -1;
	}

	/*start tcp_client thread*/
	_stop = false;
	if(pthread_create(&_thread, 0, client_thread_func, this) != 0){
		_stop = true;
		return -1;
	}

	return 0;
}

int tcp_client::connect(unsigned int ip, unsigned short port, handler *hdr){
	return _connector.connect(ip, port, hdr);
}

int tcp_client::stop() {
	/*check current tcp_client status*/
	if(_stop){
		return 0;
	}

	/*stop tcp_client thread*/
	_stop = true;
	pthread_join(_thread, 0);

	/*stop connector*/
	_connector.stop();

	/*stop workers*/
	_workers.stop();

	return 0;
}

void tcp_client::run_loop(){
	while(!_stop){
		/*wait a tiny time for next loop*/
		wait_for_next_loop();
	}
}

void tcp_client::wait_for_next_loop(){
	/*wait for 5ms*/
	::usleep(5000);
}

void* tcp_client::client_thread_func(void* arg){
	tcp_client *pclient = (tcp_client*)arg;
	pclient->run_loop();
	pthread_exit(0);
	return 0;
}
END_SERVICE_NS



