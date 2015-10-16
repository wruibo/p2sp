#ifndef _CUBE_CONTAINER_BLOCKING_DEQUE_H
#define _CUBE_CONTAINER_BLOCKING_DEQUE_H
#include <deque>
#include "cube/time/type.h"
#include "cube/define/value.h"
#include "cube/thread/thread.h"

namespace cube{
	namespace container{
		/*
		*	thread safe blocking deque class, default queue size is not limit, you
		*can use the @limit method to set the max queue size.
		*/
		template<typename T>
		class blocking_deque{
		public:
			blocking_deque(unsigned int maxsz = -1);
			virtual ~blocking_deque();

			/*
			*	push a object @obj to the tail of the queue, specify the waiting time
			*until the object has pushed into the queue
			*@param obj:
			*	object to put into the tail of the queue
			*@param waittm:
			*	which -1 means wait for ever, =0 means do not wait, >0 means actual time wait in million seconds
			*@return:
			*	0--success, <0--failed.
			*/
			int push_back(T obj, cube::time::msec_t waittm=CUBE_INFINITE);

			/*
			*	push a object @obj to the head of the queue, specify the waiting time
			*until the object has pushed into the queue
			*@param obj:
			*	object to put into the tail of the queue
			*@param waittm:
			*	which -1 means wait for ever, =0 means do not wait, >0 means actual time wait in million seconds
			*@return:
			*	0--success, <0--failed.
			*/
			int push_front(T obj, cube::time::msec_t waitmsec=CUBE_INFINITE);

			/*
			*	pop a object from the tail of the queue
			*@param obj:
			*	object popped from the tail of the queue
			*@param waittm:
			*	which -1 means wait for ever, =0 means do not wait, >0 means actual time wait in million seconds
			*return:
			*	0--success, <0--failed.
			*/
			int pop_back(T &obj, cube::time::msec_t waitmsec=CUBE_INFINITE);
			
			/*
			*	pop a object from the head of the queue
			*@param obj:
			*	object popped from the head of the queue
			*@param waittm:
			*	which -1 means wait for ever, =0 means do not wait, >0 means actual time wait in million seconds
			*return:
			*	0--success, <0--failed.
			*/
			int pop_front(T &obj, cube::time::msec_t waitmsec=CUBE_INFINITE);

		private:
			int push(T obj, cube::time::msec_t waitmsec, bool to_back);

			int pop(T &obj, cube::time::msec_t waitmsec, bool from_front);

			int push_and_wake(T obj, bool to_back);

			int pop_and_wake(T &obj, bool from_front);

		private:
			//deque
			std::deque<T> _queue;
			//max size limit for the queue
			unsigned int _maxsz;

			//mutex for deque
			cube::thread::cond_mutex_t _mutex;

			//push condition for deque
			cube::thread::condition _push_cond;
			//pop condition for deque
			cube::thread::condition _pop_cond;
		};

		template<typename T>
		blocking_deque<T>::blocking_deque(unsigned int maxsz/* = -1*/):_maxsz(maxsz)
		{
		}

		template<typename T>
		blocking_deque<T>::~blocking_deque()
		{
		}

		template<typename T>
		int blocking_deque<T>::push_back(T obj, cube::time::msec_t waitmsec)
		{
			return push(obj, waitmsec, true);
		}

		template<typename T>
		int blocking_deque<T>::push_front(T obj, cube::time::msec_t waitmsec)
		{
			return push(obj, waitmsec, false);
		}

		template<typename T>
		int blocking_deque<T>::pop_back(T &obj, cube::time::msec_t waitmsec)
		{
			return pop(obj, waitmsec, false);
		}

		template<typename T>
		int blocking_deque<T>::pop_front(T &obj, cube::time::msec_t waitmsec)
		{
			return pop(obj, waitmsec, true);
		}

		template<typename T>
		int blocking_deque<T>::push(T obj, cube::time::msec_t waitmsec, bool to_back)
		{
			cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);
			if(push_and_wake(obj, to_back) != 0 && waitmsec != 0)
			{
				/*wait for a while*/
				_push_cond.wait(&_mutex, (unsigned int)waitmsec);

				/*try to push the object to queue again*/
				return push_and_wake(obj, to_back);
			}
			
			return -1;
		}

		template<typename T>
		int blocking_deque<T>::pop(T &obj, cube::time::msec_t waitmsec, bool from_front)
		{
			cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);
			if(pop_and_wake(obj, from_front) != 0 && waitmsec != 0)
			{
				/*wait a seconds for other thread pop a object*/
				_pop_cond.wait(&_mutex);

				/*try to pop an object from the queue again*/
				return pop_and_wake(obj, from_front);
			}

			return -1;
		}

		template<typename T>
		int blocking_deque<T>::push_and_wake(T obj, bool to_back)
		{
			if(_queue.size() < _maxsz)
			{
				/*enough queue space, add to queue end*/
				if(to_back)
					_queue.push_back(obj);
				else
					_queue.push_front(obj);

				/*notify one wait thread*/
				if(_queue.size() == 1)
					_pop_cond.wakeall();

				return 0;
			}

			return -1;
		}

		template<typename T>
		int blocking_deque<T>::pop_and_wake(T &obj, bool from_front)
		{
			if(_queue.size() > 0)
			{
				if(from_front)
				{
					obj = _queue.front();
					_queue.pop_front();
				}
				else
				{
					obj = _queue.back();
					_queue.pop_back();
				}

				if(_queue.size() == _maxsz-1)
					_push_cond.wakeall();

				return 0;
			}

			return -1;
		}
	}
}
#endif
