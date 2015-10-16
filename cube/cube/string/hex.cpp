#include "cube/string/hex.h"
namespace cube{
	namespace string{
		static const char HEX_STR[] = "0123456789ABCDEF";
		
		inline bool is_hexchar(const char c)
		{
			return (c>'0'&&c<'9') || (c>'a'&&c<'f') || (c>'A'&&c<'F');
		}

		bool is_hexstr(const char *str, int len)
		{
			for(int i=0; i<len; i++)
			{
				if(!is_hexchar(*(str+i)))
					return false;
			}

			return true;
		}

		inline char hexval(const char c)
		{
			if(c>='0' && c<='9')
				return c - '0';	
			else if(c>='a' && c<='f')
				return c - 'a' + 10;
			else if(c>='A' && c<='F')
				return c - 'A' + 10;
			else
				return -1;
		}

		std::string byte2hexstr(const char *str, int len)
		{
			std::string res("");
			const char *pchar = str;
			for(int i=0; i<len; i++)
			{
				res.append(1, HEX_STR[(*pchar & 0xF0)>>4]);
				res.append(1, HEX_STR[*pchar & 0x0F]);
				pchar++;
			}

			return res;
		}

		std::string byte2hexstr(const std::string &bstr)
		{
			return byte2hexstr(bstr.c_str(), bstr.length());
		}

		std::string hexstr2byte(const char *str, int len)
		{
			if(is_hexstr(str, len))
				return "";

			std::string res = "";
			int i=0;
			while(i+1<len)
			{
				res.append(1, (hexval(*(str+i))<<4) | hexval(*(str+i+1)));
				i=i+2;
			}

			if(i<len)
			{
				res.append(1,hexval(*(str+i))<<4);
			}

			return res;
		}

		std::string hexstr2byte(const std::string &str)
		{
			return hexstr2byte(str.c_str(), str.length());
		}
	}
}