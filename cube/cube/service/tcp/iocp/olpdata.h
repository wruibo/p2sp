/*
 * olpdata.h
 *
 *  Created on: 2015年12月31日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_IOCP_OLPDATA_H_
#define CUBE_SERVICE_TCP_IOCP_OLPDATA_H_
#include <Winsock2.h>
#include "cube/service/stdns.h"
BEGIN_SERVICE_TCP_NS
class olpdata{
	typedef enum{SEND, RECV} option;
public:
	olpdata(unsigned int size);
	olpdata(const void* data, unsigned int size);
	~olpdata();

public:
	OVERLAPPED _overlapped;
	WSABUF _data;
	option _opt;
};

olpdata::olpdata(unsigned int size){
	_data.len = size;
	_data.buf = new char[size];
	_opt = olpdata::RECV;
}
olpdata::olpdata(const void* data, unsigned int size){
	_data.len = size;
	_data.buf = new char[size];
	_opt = olpdata::SEND;
	::memcpy(_data.buf, data, size);
}

olpdata::~olpdata(){
	delete []_data.buf;
	_data.len = 0;
}

END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_IOCP_OLPDATA_H_ */
