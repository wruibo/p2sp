#ifndef _CUBE_THREAD_WIN_THREAD_H
#define _CUBE_THREAD_WIN_THREAD_H
#include "cube/thread/runnable.h"
namespace cube{
	namespace thread{
		/**
		*	thread class for create/destroy a thread object
		*/
		class thread{
		public:
			thread();
			~thread();

			/**
			*	start thread with a runnable object
			*/
			int start(runnable *obj);

			/**
			*	stop the thread
			*/
			int stop();

			/**
			*	wait until the thread has terminated
			*/
			int join();

			/**
			*	get the current thread id
			*/
			unsigned int thread_id();


		private:
			/*thread function to execute the runnable instance*/
			static unsigned __stdcall thread_proxy_func(void* arg);

		private:
			//runnable instance
			runnable *_runnable_obj;

			//handle to thread
			void* _hthread;

			//stop flag for thread loop
			bool _stop;

			//thread id
			unsigned int _tid;
		};
	}
}
#endif

