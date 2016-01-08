#ifndef _CUBE_THREAD_UTIL_H
#define _CUBE_THREAD_UTIL_H
#if defined(WIN32)
#include "cube/thread/win/util.h"
#elif defined(linux) || defined(__linux__) || defined(__linux)
#include "cube/thread/linux/util.h"
#else
#error "platform is not supported."
#endif
#endif
