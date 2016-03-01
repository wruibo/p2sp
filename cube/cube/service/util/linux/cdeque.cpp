#include "cube/service/util/linux/cdeque.h"

BEGIN_SERVICE_NS
template<class type>
cdeque<type>::cdeque(){
	pthread_mutex_init(&_mutex, 0);
}

template<class type>
cdeque<type>::~cdeque(){
	pthread_mutex_destroy(&_mutex);
}

template<class type>
void cdeque<type>::push_back(type &val){
	pthread_mutex_lock(&_mutex);
	_deque.push_back(val);
	pthread_mutex_unlock(&_mutex);
}

template<class type>
void cdeque<type>::push_front(type &val){
	pthread_mutex_lock(&_mutex);
	_deque.push_front(val);
	pthread_mutex_unlock(&_mutex);
}

template<class type>
bool cdeque<type>::pop_back(type &val){
	bool empty = true;
	pthread_mutex_lock(&_mutex);
	if(!_deque.empty()){
		val = _deque.back();
		_deque.pop_back();
		empty = false;
	}
	pthread_mutex_unlock(&_mutex);
	return empty;
}

template<class type>
bool cdeque<type>::pop_front(type &val){
	bool empty = true;
	pthread_mutex_lock(&_mutex);
	if(!_deque.empty()){
		val = _deque.front();
		_deque.pop_front();
		empty = false;
	}
	pthread_mutex_unlock(&_mutex);
	return empty;
}
END_SERVICE_NS
