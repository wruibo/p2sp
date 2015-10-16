#ifndef _CUBE_THREAD_LINUX_MUTEX_H
#define _CUBE_THREAD_LINUX_MUTEX_H
#include <pthread.h>
namespace cube{
	namespace thread{
		class mutex{
			friend class condition;
		public:
			mutex();
			~mutex();

			int lock();

			int rlock();

			int wlock();

			int unlock();

		private:
			pthread_mutex_t _hmutex;
		};
	}
}
#endif
