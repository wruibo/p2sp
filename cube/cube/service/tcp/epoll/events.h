/*
 * event.h
 *
 *  Created on: 2015年12月24日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_EPOLL_EVENTS_H_
#define CUBE_SERVICE_TCP_EPOLL_EVENTS_H_
#include <sys/epoll.h>

#include "cube/service/stdns.h"
BEGIN_SERVICE_TCP_NS
/**
 * events cache for epoll wait
 */
class events{
public:
	events();
	events(int num, int wait_time);
	virtual ~events();

	int num();
	struct epoll_event* array();

	int wait_time();

private:
	//events number for epoll
	int _num;
	struct epoll_event* _array;

	//wait time for epoll wait, in milliseconds
	int _wait_time;
};
events::events(): _num(256), _wait_time(5) {
	_array = new epoll_event[_num];
}

events::~events(){
	delete []_array;
}

int events::num(){
	return _num;
}

struct epoll_event* events::array(){
	return _array;
}

int events::wait_time(){
	return _wait_time;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_EPOLL_EVENTS_H_ */
