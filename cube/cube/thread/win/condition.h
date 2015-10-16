#ifndef _CUBE_THREAD_WIN_CONDITION_H
#define _CUBE_THREAD_WIN_CONDITION_H
#include "cube/thread/win/critical_section.h"
namespace cube{
	namespace thread{
		
		typedef critical_section cond_mutex_t;

		/**
		*	condition wrap by the windows condition api
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
			CONDITION_VARIABLE _cond;
		};
	}
}
#endif
