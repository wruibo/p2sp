#ifndef _CUBE_THREAD_LINUX_SEMAPHORE_H
#define _CUBE_THREAD_LINUX_SEMAPHORE_H
#include <semaphore.h>
namespace cube{
	namespace thread{
		class semaphore{
		public:
			semaphore();
			~semaphore();

			int lock();

			int rlock();

			int wlock();

			int unlock();

		private:
			sem_t _semaphore;
		};
	}
}
#endif
