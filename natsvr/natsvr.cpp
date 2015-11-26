/*
 * natsvr.cpp
 *
 *  Created on: 2015年10月20日
 *      Author: wrb00_000
 */
#include "cube/service/udp/linux/server.h"

class myhandler: public cube::service::handler{
public:
	int handle(const cube::service::dpacket_t *req, cube::service::dpacket_t **resp, void *arg){
		return 0;
	}
};

int main(int argc, char* argv[]) {
	cube::service::server<myhandler> svc;
	svc.start(8000, 1, 0);

	return 0;
}

