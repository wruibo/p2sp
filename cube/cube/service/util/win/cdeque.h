/*
 * cqueue.h
 *
 *  Created on: 2015年12月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_
#define CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_
#include <deque>
#include <Windows.h>
#include "cube/service/stdns.h"
BEGIN_SERVICE_NS
template<class type>
class cdeque{
public:
	cdeque();
	virtual ~cdeque();

	void push_back(type &val);

	void push_front(type &val);

	bool pop_back(type &val);

	bool pop_front(type &val);

private:
	std::deque<type> _deque;
	CRITICAL_SECTION _crc;
};

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
#endif /* CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_ */
