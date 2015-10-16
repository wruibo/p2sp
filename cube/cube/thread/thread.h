#ifndef _CUBE_THREAD_THREAD_H
#define _CUBE_THREAD_THREAD_H
#include "cube/thread/util.h"
#include "cube/thread/lock.h"
#include "cube/thread/mutex.h"
#include "cube/thread/runnable.h"
#include "cube/thread/condition.h"

#if defined(WIN32)
	#include "cube/thread/win/thread.h"
#elif defined(linux) || defined(__linux__) || defined(__linux)
	#include "cube/thread/linux/thread.h"
#else
	#error "platform is not supported."
#endif

#endif

