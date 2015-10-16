#ifndef _CUBE_THREAD_LINUX_RWLOCK_H
#define _CUBE_THREAD_LINUX_RWLOCK_H
#include <pthread.h>
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
			pthread_rwlock_t _hlock;
		};
	}
}
#endif
