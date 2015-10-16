#pragma once
#include <string>

namespace cube{
	namespace string{
		std::string byte2hexstr(const char *str, int len);
		std::string byte2hexstr(const std::string &bstr);

		std::string hexstr2byte(const char *str, int len);
		std::string hexstr2byte(const std::string &hstr);
	}
}
