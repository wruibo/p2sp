#include "cube/service/tcp/iocp/olpdata.h"

BEGIN_SERVICE_TCP_NS
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
