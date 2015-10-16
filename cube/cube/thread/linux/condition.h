#ifndef _CUBE_THREAD_LINUX_CONDITION_H
#define _CUBE_THREAD_LINUX_CONDITION_H
#include <pthread.h>
#include "cube/thread/linux/mutex.h"
namespace cube{
	namespace thread{
		
		typedef mutex cond_mutex_t;

		/**
		*	condition wrap by the linux condition api
		*/
		class condition
		{
		public:
			condition();
			~condition();

			/**
			*	block current thread, wait until the condition is true
			*@param mutex: mutex relate with the condition
			*@param msec: million seconds wait for the condition
			*return:
			*	0--on success, otherwise failed
			*/
			int wait(cond_mutex_t *mutex, unsigned int msec = 0xFFFFFFFF);

			/**
			*	wake up at least 1 thread wait for the condition
			*/
			int wake();

			/**
			*	wake up all the thread wait for the condition
			*/
			int wakeall();
		
		private:
			//condition
			pthread_cond_t _cond;

			//initialize success flag
			bool _init_success;
		};
	}
}
#endif
