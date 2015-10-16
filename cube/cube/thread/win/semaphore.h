#ifndef _CUBE_THREAD_WIN_SEMAPHORE_H
#define _CUBE_THREAD_WIN_SEMAPHORE_H
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
			void* _semaphore;
		};
	}
}
#endif
