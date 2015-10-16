#ifndef _CUBE_TYPE_TYPE_H
#define _CUBE_TYPE_TYPE_H

#ifdef _WIN32
#include "cube/type/win/type.h"
#elif defined(linux) || defined(__linux__) || defined(__linux)
#include "cube/type/linux/type.h"
#else
#error "platform is not supported."
#endif

namespace cube{
	typedef unsigned char byte;
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned long ulong;
	typedef unsigned int uint;

	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned int uint32_t;
}

#endif