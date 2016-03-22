#ifndef CUBE_SERVICE_TCP_IOCP_OLPDATA_H_
#define CUBE_SERVICE_TCP_IOCP_OLPDATA_H_
#include <WinSock2.h>
#include "cube/service/stdns.h"
BEGIN_SERVICE_TCP_NS
class olpdata{
public:
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
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_IOCP_OLPDATA_H_ */
