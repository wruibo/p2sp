#ifndef CUBE_STRING_CONVERT_H_
#define CUBE_STRING_CONVERT_H_
#if defined(WIN32)
#include "cube/string/win/convert.h"
#else if define(linux)
#include "cube/string/linux/convert.h"
#else
#error "platform is not support."
#endif
