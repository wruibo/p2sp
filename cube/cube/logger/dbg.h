#pragma once
namespace cube{
	namespace logger{
#ifdef _DEBUG
#define _DBG_OPEN
#endif

#ifdef _DBG_OPEN
#ifdef _DBG_DETAIL
#define DBG_INFO(x) XDBG_INFO(x, true)
#define DBG_WARNING(x) XDBG_WARNING(x, true)
#define DBG_ERROR(x) XDBG_ERROR(x, true)
#define DBG_FATAL(x) XDBG_FATAL(x, true)
#else
#define DBG_INFO(x) XDBG_INFO(x, false)
#define DBG_WARNING(x) XDBG_WARNING(x, false)
#define DBG_ERROR(x) XDBG_ERROR(x, false)
#define DBG_FATAL(x) XDBG_FATAL(x, false)
#endif //end _DBG_DETAIL

#define XDBG_INFO(x, file_flag) do { \
	do{\
	time_t lt = time(NULL); \
		struct tm *ptr = localtime(&lt);\
		char str[64] = {0}; \
		strftime(str,63,"[%Y%m%d%H%M%S][info]: ", ptr); \
		printf(str);\
	}while(0); \
	printf x ; \
	printf("\r\n"); \
	if(file_flag) \
	printf("[ file: %s, line: %d ]\r\n", __FILE__, __LINE__);\
	fflush(stdout); \
	} while(0)

#define XDBG_WARNING(x, file_flag) do { \
	do{\
	time_t lt = time(NULL); \
		struct tm *ptr = localtime(&lt); \
		char str[64] = {0}; \
		strftime(str,63,"[%Y%m%d%H%M%S][warning]: ", ptr); \
		printf(str); \
	}while(0);\
	printf x ; \
	printf("\r\n"); \
	if(file_flag) \
	printf("[ file: %s, line: %d ]\r\n", __FILE__, __LINE__);\
	fflush(stdout); \
	} while(0)

#define XDBG_ERROR(x, file_flag) do { \
	do{\
	time_t lt = time(NULL); \
		struct tm *ptr = localtime(&lt); \
		char str[64] = {0}; \
		strftime(str,63,"[%Y%m%d%H%M%S][error]: ", ptr); \
		printf(str); \
	}while(0);\
	printf x ; \
	printf("\r\n"); \
	if(file_flag) \
	printf("[ file: %s, line: %d ]\r\n", __FILE__, __LINE__);\
	fflush(stdout); \
	} while(0)

#define XDBG_FATAL(x, file_flag) do { \
	do{\
	time_t lt = time(NULL); \
		struct tm *ptr = localtime(&lt); \
		char str[64] = {0}; \
		strftime(str,63,"[%Y%m%d%H%M%S][fatal]: ", ptr); \
		printf(str); \
	}while(0);\
	printf x ; \
	printf("\r\n"); \
	if(file_flag) \
	printf("[ file: %s, line: %d ]\r\n", __FILE__, __LINE__);\
	fflush(stdout); \
	} while(0)
#else
#define	DBG_INFO(x)
#define	XDBG_INFO(x, file_flag)
#define	DBG_WARNING(x)
#define	XDBG_WARNING(x, file_flag)
#define	DBG_ERROR(x)
#define	XDBG_ERROR(x, file_flag)
#define DBG_FATAL(x)
#define	XDBG_FATAL(x, file_flag)
#endif /* DEBUG */
	}
}
