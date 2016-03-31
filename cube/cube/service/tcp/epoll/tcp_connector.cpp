#include "cube/service/tcp/tcp_connector.h"

BEGIN_SERVICE_NS
tcp_connector::tcp_connector() : _workers(0), _pending_handlers_mutex(0), _epoll(-1), _thread(0), _stop(true) {
	pthread_mutex_init(&_pending_handlers_mutex, 0);
}

tcp_connector::~tcp_connector(){
	pthread_mutex_destroy(&_pending_handlers_mutex);
}

int tcp_connector::start(workers *workers){
	if(!_stop){ //tcp_connector has been started
		return 0;
	}
	_workers = workers;

	/*create epoll for tcp_connector*/
	_epoll = epoll_create(256);
	if(_epoll < 0){
		return -1;
	}

	/*start the tcp_connector thread*/
	_stop = false;
	if(pthread_create(&_thread, 0, connect_thread_func, this) != 0)
	{
		_stop = true;
		return -1;
	}

	return 0;
}

int tcp_connector::connect(unsigned int ip, unsigned short port, handler* hdr){
	/*create socket*/
	int sock = async_tcp_connect(ip, port);
	if(sock < 0){
		return -1;
	}

	/*set the session property of the handler*/
	hdr->sock(sock);
	hdr->remote_ip(ip);
	hdr->remote_port(port);

	/*track the connect result for the handler*/
	if(this->track(hdr) != 0){
		return -1;
	}

	return 0;
}

int tcp_connector::stop(){
	if(_stop){
		return -1;
	}

	/*stop connect thread*/
	_stop = true;
	pthread_join(_thread, 0);

	/*free handler resources*/
	this->free();

	/*close epoll handler*/
	::close(_epoll);

	return 0;
}

int tcp_connector::track(handler *hdr){
	/*add the handler to epoll*/
	struct epoll_event ev;
	ev.events = EPOLLOUT|EPOLLET;
	ev.data.ptr = hdr;
	if(epoll_ctl(_epoll, EPOLL_CTL_ADD, hdr->sock(), &ev) < 0){
		return -1;
	}

	/*add the handler to pending list*/
	pthread_mutex_lock(&_pending_handlers_mutex);
	_pending_handlers.push_back(hdr);
	pthread_mutex_unlock(&_pending_handlers_mutex);
	return 0;
}

void tcp_connector::untrack(handler *hdr){
	/*remove handler from epoll*/
	epoll_ctl(_epoll, EPOLL_CTL_DEL, hdr->sock(), 0);

	/*remote handler from pending list*/
	pthread_mutex_lock(&_pending_handlers_mutex);
	_pending_handlers.remove(hdr);
	pthread_mutex_unlock(&_pending_handlers_mutex);
}

void tcp_connector::dispatch(handler *hdr){
	if(_workers->dispatch(hdr) != 0){
		hdr->on_close(ERR_DISPATCH_FAILED);
		delete hdr;
	}
}

void tcp_connector::free(){
	pthread_mutex_lock(&_pending_handlers_mutex);
	std::list<handler*>::iterator iter = _pending_handlers.begin(), iterend = _pending_handlers.end();
	while(iter != iterend){
		(*iter)->on_close(ERR_TERMINATE_SESSION);
		delete *iter;
		iter++;
	}
	_pending_handlers.clear();
	pthread_mutex_unlock(&_pending_handlers_mutex);
}

void tcp_connector::process_tracked_handlers(){
	/*wait for epoll event*/
	int num = epoll_wait(_epoll, _events.array(), _events.num(), _events.wait_time());
	for(int i=0; i<num; i++)
	{
		handler *hdr = (handler*)_events.array()[i].data.ptr;
		/*remove the handler from track list*/
		this->untrack(hdr);
		if(_events.array()[i].events & EPOLLOUT){/*connect succeed*/
			/*dispatch the connected handler to workers*/
			this->dispatch(hdr);
		} else {/*connect failed*/
			/*call the handler to deal with error*/
			hdr->on_close(ERR_CONNECT_FAILED);
			/*free the handler*/
			delete hdr;
		}
	}
}

void tcp_connector::wait_for_next_loop() {
	::usleep(5000);
}

void tcp_connector::run_loop(){
	while(!_stop){
		/*process the tracked handlers*/
		this->process_tracked_handlers();

		/*wait for the next loop*/
		this->wait_for_next_loop();
	}
}
void* tcp_connector::connect_thread_func(void* arg){
	tcp_connector* pconnector = (tcp_connector*) arg;
	pconnector->run_loop();
	pthread_exit(0);
	return 0;
}
END_SERVICE_NS




