#pragma once
#include <string>

namespace cube{
	namespace string{
		/*trim blank characters in the string*/
		extern std::string trim(const char *str);
		extern std::string trim_left(const char *str);
		extern std::string trim_right(const char *str);
	}
}
