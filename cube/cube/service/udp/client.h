/*
 * client.h
 *
 *  Created on: 2015年11月26日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UDP_CLIENT_H_
#define CUBE_SERVICE_UDP_CLIENT_H_

#if defined(linux) || defined(__linux__) || defined(__linux)
#include <cube/service/udp/client/linux/client.h>
#elif defined(WIN32) || defined(WIN)
#include "cube/service/udp/win/client.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_UDP_CLIENT_H_ */
