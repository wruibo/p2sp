#ifndef _CUBE_THREAD_CONDITION_H
#define _CUBE_THREAD_CONDITION_H
#if defined(WIN32)
#include "cube/thread/win/condition.h"
#elif defined(linux) || defined(__linux__) || defined(__linux)
#include "cube/thread/linux/condition.h"
#else
#error "platform is not supported."
#endif
#endif
