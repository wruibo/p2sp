#ifndef _CUBE_THREAD_THREAD_EXCEPTION_H
#define _CUBE_THREAD_THREAD_EXCEPTION_H
#include <string>
#include <exception>
#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
class thread_exception: public std::exception {
public:
	thread_exception() throw(): _msg("") {}

	thread_exception(const char *msg) throw()
	{
		_msg.assign(msg);
	}

	thread_exception(const std::string &msg) throw()
	{
		_msg = msg;
	}

	~thread_exception() throw()
	{

	}

	const char * what() const throw()
	{
		return _msg.c_str();
	}

private:
	//save the exception message
	std::string _msg;
};
END_THREAD_NS
#endif
