#ifndef _CUBE_THREAD_MUTEX_H
#define _CUBE_THREAD_MUTEX_H

#if defined(WIN32)
#include "cube/thread/win/mutex.h"
#include "cube/thread/win/rwlock.h"
#include "cube/thread/win/semaphore.h"
#include "cube/thread/win/critical_section.h"
#elif defined(linux) || defined(__linux__) || defined(__linux)
#include "cube/thread/linux/mutex.h"
#include "cube/thread/linux/rwlock.h"
#include "cube/thread/linux/semaphore.h"
#include "cube/thread/linux/critical_section.h"
#else
#error "platform is not supported."
#endif

#endif
