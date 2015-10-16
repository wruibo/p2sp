#include <Windows.h>
#include "cube/string/codecvt.h"

namespace cube{
	namespace string{		
		std::wstring locale2unicode(const std::string &locale_str)
		{
			int sz = locale_str.length();
			int wsz = sz+1;
			wchar_t *wstr = new wchar_t[sz];
			int num = MultiByteToWideChar(CP_ACP, 0, locale_str.c_str(), sz, wstr, wsz);

			std::wstring ret(wstr, num);
			delete []wstr;
			return ret;
		}

		std::wstring utf82unicode(const std::string &utf8_str)
		{
			int sz = utf8_str.length();
			int wsz = 2*(sz+1);
			wchar_t *wstr = new wchar_t[sz];
			int num = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), sz, wstr, wsz);

			std::wstring ret(wstr, num);
			delete []wstr;
			return ret;
		}

		std::wstring gbk2unicode(const std::string &gbk_str)
		{
			int sz = gbk_str.length();
			int wsz = 2*(sz+1);
			wchar_t *wstr = new wchar_t[sz];
			int num = MultiByteToWideChar(936, 0, gbk_str.c_str(), sz, wstr, wsz);

			std::wstring ret(wstr, num);
			delete []wstr;
			return ret;
		}

		std::string unicode2locale(const std::wstring &unicode_str)
		{
			size_t wsz = unicode_str.length();
			size_t sz = 4*wsz;
			char *str = new char[sz];
			int num = WideCharToMultiByte(CP_ACP, 0, unicode_str.c_str(), wsz, str, sz, 0, 0);

			std::string ret(str, num);
			delete []str;
			return ret;
		}

		std::string unicode2utf8(const std::wstring &unicode_str)
		{
			size_t wsz = unicode_str.length();
			size_t sz = 4*wsz;
			char *str = new char[sz];
			int num = WideCharToMultiByte(CP_UTF8, 0, unicode_str.c_str(), wsz, str, sz, 0, 0);

			std::string ret(str, num);
			delete []str;
			return ret;
		}

		std::string unicode2gb2312(const std::wstring &unicode_str)
		{
			return unicode2gbk(unicode_str);
		}

		std::string unicode2gbk(const std::wstring &unicode_str)
		{
			size_t wsz = unicode_str.length();
			size_t sz = 4*wsz;
			char *str = new char[sz];
			int num = WideCharToMultiByte(936, 0, unicode_str.c_str(), wsz, str, sz, 0, 0);

			std::string ret(str, num);
			delete []str;
			return ret;
		}

		std::string unicode2ansi(const std::wstring &unicode_str)
		{
			size_t wsz = unicode_str.length();
			size_t sz = 4*wsz;
			char *str = new char[sz];
			int num = WideCharToMultiByte(936, 0, unicode_str.c_str(), wsz, str, sz, 0, 0);

			std::string ret(str, num);
			delete []str;
			return ret;
		}
	}
}
