#ifndef CUBE_SERVICE_UTIL_WIN_CDEQUE_H_
#define CUBE_SERVICE_UTIL_WIN_CDEQUE_H_
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
END_SERVICE_NS
#endif /* CUBE_SERVICE_UTIL_LINUX_CDEQUE_H_ */
