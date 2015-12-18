/*
 * network.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_STDSVC_H_
#define CUBE_SERVICE_STDSVC_H_

#if defined(__linux__)
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#elif defined(__WIN32__)
#include <WinSock2.h>
#include <Windows.h>
#else
#error "platform is not supported"
#endif


#endif /* CUBE_SERVICE_STDSVC_H_ */
