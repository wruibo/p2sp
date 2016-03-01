#include "cube/service/tcp/iocp/workers.h"

BEGIN_SERVICE_TCP_NS
workers::workers():_wnum(0), _pos(0){

}
workers::~workers(){

}

int workers::start(int num, void* arg/* = NULL*/){
	if(num < 1){
		return -1;
	}
	_wnum = num;

	for(int i=0; i<_wnum; i++){
		worker* pworker = new worker();
		if(pworker->start(arg) != 0){
			return -1;
		}
		_workers.push_back(pworker);
	}
	return 0;
}

int workers::dispatch(handler *hdr){
	_workers[_pos++%_wnum]->dispatch(hdr);
	return 0;
}

int workers::stop(){
	for(int i=0; i<_wnum; i++){
		_workers[i]->stop();
		delete _workers[i];
	}
	_workers.clear();
	return 0;
}
END_SERVICE_TCP_NS
