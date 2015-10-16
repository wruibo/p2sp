#ifndef _CUBE_THREAD_WIN_RWLOCK_H
#define _CUBE_THREAD_WIN_RWLOCK_H
#include <Windows.h>
#include "cube/thread/win/critical_section.h"
namespace cube{
	namespace thread{
		class rwlock{
		public:
			rwlock();
			~rwlock();

			int lock();

			int rlock();

			int wlock();

			int unlock();

		private:
			//slim read-write lock
			SRWLOCK _rwlock;
			//flag indicate current lock type
			bool _islock_shared;
		};
	}
}
#endif
