#include <process.h>

#include "cube/service/tcp/iocp/client.h"
BEGIN_SERVICE_TCP_NS
client::client(): _thread(NULL), _thread_id(0), _stop(true) {

}

client::~client() {
}

int client::start(int workers) {
	/*check if client has been started*/
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

	/*start client thread*/
	_stop = false;
	_thread = (HANDLE)::_beginthreadex(NULL, 0, client_thread_func, this, 0, &_thread_id);
	if(_thread == NULL){
		_stop = true;
		return -1;
	}

	return 0;
}

int client::build(unsigned int ip, unsigned short port, handler *hdr){
	return _connector.connect(ip, port, hdr);
}

int client::stop() {
	/*check current client status*/
	if(_stop){
		return 0;
	}

	/*stop client thread*/
	_stop = true;
	::WaitForSingleObject(_thread, INFINITE);
	::CloseHandle(_thread);

	/*stop connector*/
	_connector.stop();

	/*stop workers*/
	_workers.stop();

	return 0;
}

void client::wait_for_next_loop(){
	/*wait for 5ms*/
	::Sleep(5);
}

void client::run_loop(){
	while(!_stop){
		/*wait a tiny time for next loop*/
		wait_for_next_loop();
	}
}

unsigned client::client_thread_func(void* arg){
	client *pclient = (client*)arg;
	pclient->run_loop();
	::_endthreadex(0);
	return 0;
}
END_SERVICE_TCP_NS
