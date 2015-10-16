#ifndef _CUBE_THREAD_WIN_CRITICAL_SECTION_H
#define _CUBE_THREAD_WIN_CRITICAL_SECTION_H
#include <windows.h>
namespace cube{
	namespace thread{
		class critical_section{
			friend class condition;
		public:
			critical_section();
			~critical_section();

			int lock();

			int rlock();

			int wlock();

			int unlock();

		private:
			CRITICAL_SECTION _cs;
		};
	}
}
#endif
