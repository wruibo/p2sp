#ifndef _CUBE_THREAD_WIN_MUTEX_H
#define _CUBE_THREAD_WIN_MUTEX_H
namespace cube{
	namespace thread{
		class mutex{
		public:
			mutex();
			~mutex();

			int lock();

			int rlock();

			int wlock();

			int unlock();

		private:
			void* _hmutex;
		};
	}
}
#endif
