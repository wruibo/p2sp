#include "cube/service/util/win/cdeque.h"

BEGIN_SERVICE_NS
template<class type>
cdeque<type>::cdeque(){
	InitializeCriticalSection(&_crc);
}

template<class type>
cdeque<type>::~cdeque(){
	DeleteCriticalSection(&_crc);
}

template<class type>
void cdeque<type>::push_back(type &val){
	EnterCriticalSection(&_crc);
	_deque.push_back(val);
	LeaveCriticalSection(&_crc);
}

template<class type>
void cdeque<type>::push_front(type &val){
	EnterCriticalSection(&_crc);
	_deque.push_front(val);
	LeaveCriticalSection(&_crc);
}

template<class type>
bool cdeque<type>::pop_back(type &val){
	bool empty = true;
	EnterCriticalSection(&_crc);
	if(!_deque.empty()){
		val = _deque.back();
		_deque.pop_back();
		empty = false;
	}
	LeaveCriticalSection(&_crc);
	return empty;
}

template<class type>
bool cdeque<type>::pop_front(type &val){
	bool empty = true;
	EnterCriticalSection(&_crc);
	if(!_deque.empty()){
		val = _deque.front();
		_deque.pop_front();
		empty = false;
	}
	LeaveCriticalSection(&_crc);
	return empty;
}
END_SERVICE_NS
