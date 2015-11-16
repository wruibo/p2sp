#include "cube/time/time.h"
#include "cube/time/systm.h"

namespace cube{
	namespace time{
		msec_t now()
		{
			return cube::time::mtime(0);
		}

		std::string now(const char* format)
		{
			time_t tnow = ::time(0);
			struct tm* tmnow = ::localtime(&tnow);

			const int LENGTH = 512;
			char str[LENGTH] = {0};
			size_t len = ::strftime(str, LENGTH, format, tmnow);

			return std::string(str, len);
		}
	}
}
