#ifndef _CUBE_THREAD_LOCK_H
#define _CUBE_THREAD_LOCK_H
namespace cube{
	namespace thread{
		/**
		*	shared lock for mutex
		*/
		template <class T> 
		class shared_lock
		{
		public:
			explicit shared_lock(T &mutex):_mutex(&mutex)
			{
				_mutex->rlock();
			}

			~shared_lock()
			{
				_mutex->unlock();
			}

		private:
			T *_mutex;
		};

		/**
		*	unique lock for mutex
		*/
		template <class T> 
		class unique_lock
		{
		public:
			explicit unique_lock(T &mutex):_mutex(&mutex)
			{
				_mutex->wlock();
			}

			~unique_lock()
			{
				_mutex->unlock();
			}

		private:
			T *_mutex;
		};

		/**
		*	scope lock for mutex, like unique lock
		*/
		template <class T> 
		class scope_lock
		{
		public:
			explicit scope_lock(T &mutex):_mutex(&mutex)
			{
				_mutex->lock();
			}

			~scope_lock()
			{
				_mutex->unlock();
			}

		private:
			T *_mutex;
		};
	}
}
#endif
