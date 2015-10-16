#pragma once
#include <string>

namespace cube{
	namespace string{
		extern std::string to_upper(const std::string &instr);
		extern std::string to_lower(const std::string &instr);
		extern std::wstring to_upper(const std::wstring &instr);
		extern std::wstring to_lower(const std::wstring &instr);
	}
}