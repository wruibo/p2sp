/*
 * cqueue.h
 *
 *  Created on: 2015年12月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_
#define CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_
#include <deque>
#include <pthread.h>
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
	pthread_mutex_t _mutex;
};
END_SERVICE_NS
#endif /* CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_ */
