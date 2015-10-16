#include "cube/time/systm.h"
#include <Windows.h>

namespace cube{
	namespace time{

		/**
		*	translate the "struct tm" to "SYSTEMTIME"
		*/
		SYSTEMTIME tm2systime(struct tm* tm)
		{
			SYSTEMTIME st;
			st.wYear = tm->tm_year;
			st.wMonth = tm->tm_mon+1;
			st.wDay = tm->tm_mday;
			st.wDayOfWeek = tm->tm_wday;
			st.wHour = tm->tm_hour;
			st.wMinute = tm->tm_min;
			st.wSecond = tm->tm_sec;
			st.wMilliseconds = 0;

			return st;
		}
		
		/**
		*	get the utc file time in million seconds since 1601.01.01
		*@return:
		*	million seconds of utc time since 1601.01.01
		*/
		msec_t mftime(struct tm* tm/* = 0*/)
		{
			FILETIME ftm;
			if(tm != 0){
				SYSTEMTIME st = tm2systime(tm);
				SystemTimeToFileTime(&st, &ftm);
			}else{
				GetSystemTimeAsFileTime(&ftm);
			}
			return *((uint64_t*)&ftm)/10000;
		}

		/**
		*	get the file time in million seconds at 1970.1.1
		*/
		msec_t mftime19700101()
		{
			struct tm t1970;
			memset(&t1970, 0, sizeof(struct tm));

			t1970.tm_year = 1970;
			t1970.tm_mon = 0;
			t1970.tm_mday = 1;
			
			return mftime(&t1970);
		}

		/**
		*	get the unix timestamp of specified @tm in million seconds,
		*if @tm is null,then return the current epoch time in million seconds
		*@return:
		*	million seconds of the specified @tm
		*/
		msec_t mtime(struct tm* tm/* = 0*/)
		{
			return mftime(tm)-mftime19700101();
		}
	}
}
