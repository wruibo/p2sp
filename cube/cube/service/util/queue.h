/*
 * queue.h
 *
 *  Created on: 2016年1月4日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_QUEUE_H_
#define CUBE_SERVICE_UTIL_QUEUE_H_

#if defined(__linux__)
#include "cube/service/util/linux/queue.h"
#elif defined(WIN32)
#include "cube/service/util/win/queue.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_UTIL_QUEUE_H_ */
