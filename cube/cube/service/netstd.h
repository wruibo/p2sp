/*
 * network.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_NETSTD_H_
#define CUBE_SERVICE_NETSTD_H_

#if defined(__linux__)
#include <arpa/inet.h>
#include <sys/socket.h>
#elif defined(__WIN32__)
#include <WinSock2.h>
#include <Windows.h>
#else
#error "platform is not supported"
#endif


#endif /* CUBE_SERVICE_NETSTD_H_ */
