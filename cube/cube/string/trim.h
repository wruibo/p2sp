#ifndef CUBE_STRING_TRIM_H_
#define CUBE_STRING_TRIM_H_
#include <string>
#include <string.h>
#include "cube/string/trim.h"
#include "cube/string/stdns.h"
BEGIN_STRING_NS
std::string trim(const char *str)
{
	int str_len = (int)strlen(str);

	int i=0;
	const char *p = str;
	for(i=0; i<str_len; i++)
	{
		if(*(p+i) != ' ' && *(p+i) != '\t' && *(p+i) != '\r' && *(p+i) != '\n')
		break;
	}

	int start = i;
	p = str;
	for(i=str_len-1; i>=0; i--)
	{
		if(*(p+i) != ' ' && *(p+i) != '\t' && *(p+i) != '\r' && *(p+i) != '\n')
		break;
	}

	int count = i - start + 1;
	std::string ret;
	if(count <= 0)
	ret = "";
	else
	ret = std::string(str+start, count);

	return ret;
}

std::string trim_left(const char *str)
{
	int str_len = (int)strlen(str);
	int i=0;
	const char *p = str;
	for(i=0; i<str_len; i++)
	{
		if(*(p+i) != ' ' && *(p+i) != '\t' && *(p+i) != '\r' && *(p+i) != '\n')
		break;
	}

	int start = i;
	std::string ret;
	if(start > str_len -1)
	ret = "";
	else
	ret = std::string(str+start);
	return ret;
}

std::string trim_right(const char *str)
{
	int str_len = (int)strlen(str);
	int i=0;
	const char *p = str;
	p = str;
	for(i=str_len-1; i>=0; i--)
	{
		if(*(p+i) != ' ' && *(p+i) != '\t' && *(p+i) != '\r' && *(p+i) != '\n')
		break;
	}

	int end = i;

	std::string ret;
	if(end < 0)
	ret = "";
	else
	ret = std::string(str, end+1);
	return ret;
}
END_STRING_NS
#endif

