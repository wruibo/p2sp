#ifndef _CUTE_THREAD_RUNNABLE_H
#define _CUTE_THREAD_RUNNABLE_H
namespace cube{
	namespace thread{
		/**
		*	runnable interface for thread 
		*/
		class runnable{
		public:
			/**
			*	runnable loop body, this method will execute repeatedly
			*until the thread has stopped
			*/
			virtual void loop() = 0;
		};
	}
}
#endif

