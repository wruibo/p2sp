#pragma once
#include <string>

namespace cube{
	namespace string{
		extern std::wstring locale2unicode(const std::string &locale_str);
		extern std::wstring utf82unicode(const std::string &utf8_str);
		extern std::wstring gbk2unicode(const std::string &gbk_str);
		extern std::string unicode2locale(const std::wstring &unicode_str);
		extern std::string unicode2utf8(const std::wstring &unicode_str);
		extern std::string unicode2gbk(const std::wstring &unicode_str);
		extern std::string unicode2gb2312(const std::wstring &unicode_str);
		extern std::string unicode2ansi(const std::wstring &unicode_str);
	}
}

