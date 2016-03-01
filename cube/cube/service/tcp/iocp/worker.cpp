#include "cube/service/tcp/iocp/worker.h"

BEGIN_SERVICE_TCP_NS
worker::worker(): _arg(NULL), _iocp(INVALID_HANDLE_VALUE), _thread(INVALID_HANDLE_VALUE), _thread_id(0), _stop(true){
}

worker::~worker(void) {
}

int worker::start(void* arg /*= NULL*/) {
	/*set the argument that will be passed to handler*/
	_arg = arg;

	/*create io complete port*/
	_iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if(_iocp == NULL){
		return -1; //create iocp failed.
	}

	/*start worker thread*/
	_stop = false;
	_thread = (HANDLE)::_beginthreadex(NULL, 0, work_thread_func, this, 0, &_thread_id);
	if(_thread == NULL){
		_stop = true;
		return -1;
	}

	return 0;
}

void worker::dispatch(handler *hd) {
	_pending_handlers.push_back(hd);
}

int worker::stop() {
	if(_stop){
		return -1;
	}

	/*stop worker thread*/
	_stop = true;
	::WaitForSingleObject(_thread, INFINITE);
	::CloseHandle(_thread);

	/*free all handlers*/
	this->free();

	/*close iocp handle*/
	::CloseHandle(_iocp);

	return 0;
}

void worker::remove(SOCKET sock) {
	map<int, handler*>::iterator iter = _processing_handlers.find(sock);
	if (iter != _processing_handlers.end()) {
		_processing_handlers.erase(iter);
	}
}

void worker::free() {
	/*free processing handlers*/
	map<int, handler*>::iterator iter = _processing_handlers.begin(), iterend = _processing_handlers.end();
	while(iter != iterend){
		iter->second->on_close(ERR_TERMINATE_SESSION);
		delete iter->second;
		iter++;
	}
	_processing_handlers.clear();

	/*free pending handlers*/
	handler *hdr = 0;
	while (_pending_handlers.pop_front(hdr)) {
		hdr->on_close(ERR_TERMINATE_SESSION);
		delete hdr;
	}
}

void worker::accept_pending_handlers() {
	handler *hdr = 0;
	while(_pending_handlers.pop_front(hdr)){
		if (hdr->on_open(_arg) != 0) {
			/*recall on open failed*/
			hdr->on_close(ERR_TERMINATE_SESSION);
			delete hdr;
		} else {
			/*add the handler to iocp*/
			if (::CreateIoCompletionPort((HANDLE)hdr->sock(), _iocp, (ULONG_PTR)hdr, 0) == NULL) {
				hdr->on_close(ERR_IOCP_ADD_FAILED);
				delete hdr;
			} else{
				_processing_handlers.insert(pair<int, handler*>(hdr->sock(), hdr));
			}
		}
	}
}

void worker::poll_processing_handlers() {
	DWORD transfered = 0;
	handler *hdr = 0;
	olpdata *olp = 0;
	while(true){
		if(::GetQueuedCompletionStatus(_iocp, &transfered, (PULONG_PTR)&hdr, (LPOVERLAPPED*)&olp, 5)){
			if(olp->_opt == olpdata::RECV){
				/*receiving data has completed*/
				if(hdr->on_recv(olp->_data.buf, transfered) != 0){
					hdr->on_close(ERR_TERMINATE_SESSION);
					this->remove(hdr->sock());
					delete hdr;
				}
			}else{
				/*sending data has completed*/
				if(hdr->on_send(transfered) != 0){
					hdr->on_close(ERR_TERMINATE_SESSION);
					this->remove(hdr->sock());
					delete hdr;
				}
			}
			/*free overlapped object*/
			delete olp;
		}else{
			if(olp == 0){/*may be timeout*/
				if(WSAGetLastError() == WSA_WAIT_TIMEOUT){
					break;
				}
			} else { /*something error happened*/
				/*free handler*/
				hdr->on_close(ERR_TERMINATE_SESSION);
				delete hdr;

				/*free overlapped object*/
				delete olp;
			}
		}
	}


}

void worker::run_processing_handlers() {
	/*get the run time*/
	time_t now = time(0);

	/*run all handlers*/
	map<int, handler*>::iterator iter = _processing_handlers.begin(), iter_end = _processing_handlers.end();
	while (iter != iter_end) {
		handler *hdr = (handler*)iter->second;
		/*check the timer of each handler first*/
		if (hdr->is_timeout(now)) {
			if (hdr->on_timeout(now) != 0) {
				hdr->on_close(ERR_TERMINATE_SESSION);
				_processing_handlers.erase(iter++);
				delete hdr;
				continue;
			}
		}

		/*recall the handler run of each handler*/
		if (hdr->on_running(now) != 0) {
			hdr->on_close(ERR_TERMINATE_SESSION);
			_processing_handlers.erase(iter++);
			delete hdr;
			continue;
		} else{
			iter++;
		}
	}
}

void worker::run_loop(){
	while (!_stop) {
		/*accept the new handlers in the pending map*/
		this->accept_pending_handlers();

		/*poll events from epoll and process io operation*/
		this->poll_processing_handlers();

		/*run every processing handler, inlcude timer operation*/
		this->run_processing_handlers();
	}

	_endthreadex(0);
}

unsigned worker::work_thread_func(void *arg) {
	worker* pworker = (worker*) arg;
	pworker->run_loop();
	return 0;
}
END_SERVICE_TCP_NS
