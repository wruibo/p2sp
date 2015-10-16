#include <locale>
#include "cube/string/case.h"
namespace cube{
	namespace string{
		std::string to_lower(const std::string &instr)
		{
			std::locale loc;

			std::string lower_str("");

			for(size_t i=0; i<instr.length(); i++)
			{
				lower_str.append(1, std::tolower(instr.at(i), loc));
			}

			return lower_str;
		}

		std::string to_upper(const std::string &instr)
		{
			std::locale loc;

			std::string upper_str("");

			for(size_t i=0; i<instr.length(); i++)
			{
				upper_str.append(1, std::toupper(instr.at(i), loc));
			}

			return upper_str;
		}

		std::wstring to_lower(const std::wstring &instr)
		{
			std::locale loc;

			std::wstring lower_str(L"");

			for(size_t i=0; i<instr.length(); i++)
			{
				lower_str.append(1, std::tolower(instr.at(i), loc));
			}

			return lower_str;
		}

		std::wstring to_upper(const std::wstring &instr)
		{
			std::locale loc;

			std::wstring upper_str(L"");

			for(size_t i=0; i<instr.length(); i++)
			{
				upper_str.append(1, std::toupper(instr.at(i), loc));
			}

			return upper_str;
		}
	}
}
