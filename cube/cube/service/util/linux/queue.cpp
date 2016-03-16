#include <time.h>
#include "cube/service/util/linux/queue.h"

BEGIN_SERVICE_NS
template<class type>
queue<type>::queue() :
		_qsize(0), _queue(0), _rwflag(0), _wwflag(0), _rpos((unsigned long) -1), _wpos((unsigned long) -1) , _cond1(0), _cond2(0){

}

template<class type>
queue<type>::queue(unsigned long qsz) {
	/*initial value of queue*/
	_qsize = roundup_power_of_two(qsz);

	/*allocate the queue buffer*/
	_queue = new type[_qsize];

	/*initialize the read&write wait flag*/
	_rwflag = 1;
	_wwflag = 0;

	/*initialize the read&write position*/
	_rpos = 0;
	_wpos = 0;

	/*initialize the semaphore variable*/
	int err = sem_init(&_cond1, 0, 0);
	if (err != 0)
		std::cout << "initial cycled read&write queue condition1 failed." << std::endl;

	err = sem_init(&_cond2, 0, 0);
	if (err != 0)
		std::cout << "initial cycled read&write queue condition2 failed." << std::endl;
}

template<class type>
queue<type>::~queue() {
	if (_queue != 0)
		delete[] _queue;
	_queue = 0;

	sem_destroy(&_cond1);
	sem_destroy(&_cond2);
}

template<class type>
int queue<type>::init(unsigned long qsz) {
	/*initial value of queue*/
	_qsize = roundup_power_of_two(qsz);

	/*allocate the queue buffer*/
	_queue = new type[_qsize];
	if (_queue == 0)
		return -1;

	/*initialize the read&write wait flag*/
	_rwflag = 1;
	_wwflag = 0;

	/*initialize the read&write position*/
	_rpos = 0;
	_wpos = 0;

	/*initialize the semaphore variable*/
	int err = sem_init(&_cond1, 0, 0);
	if (err != 0)
		std::cout << "initial cycled read&write queue condition1 failed." << std::endl;

	err = sem_init(&_cond2, 0, 0);
	if (err != 0)
		std::cout << "initial cycled read&write queue condition2 failed." << std::endl;

	return 0;
}

template<class type>
int queue<type>::read(type *ptr, int waittm/*=-1*/) {
	int wtm = 10;
	if (!(waittm < 0))
		wtm = waittm;

	int err = _read(ptr);
	while (err != 0) {
		_rwflag = 1;
		if (_wwflag == 1) {
			_wwflag = 0;
			sem_post(&_cond2);
		}

		struct timespec tw;
		get_waittm(&tw, wtm);
		int res = sem_timedwait(&_cond1, &tw);
		if (res == -1 && !(waittm < 0) && errno == ETIMEDOUT) {
			err = _read(ptr);
			break;
		}

		err = _read(ptr);
	}

	if (_wwflag == 1) {
		_wwflag = 0;
		sem_post(&_cond2);
	}

	if (err != 0)
		return -1;

	return 0;
}

template<class type>
int queue<type>::write(type ptr, int waittm/*=-1*/) {
	int wtm = 10;
	if (!(waittm < 0))
		wtm = waittm;

	int err = _write(ptr);
	while (err != 0) {
		_wwflag = 1;
		if (_rwflag == 1) {
			_rwflag = 0;
			sem_post(&_cond1);
		}

		struct timespec tw;
		get_waittm(&tw, wtm);
		int res = sem_timedwait(&_cond2, &tw);
		if (res == -1 && !(waittm < 0) && errno == ETIMEDOUT) {
			err = _write(ptr);
			break;
		}

		err = _write(ptr);
	}

	if (_rwflag == 1) {
		_rwflag = 0;
		sem_post(&_cond1);
	}

	if (err != 0)
		return -1;

	return 0;
}

template<class type>
int queue<type>::_read(type *ptr) {
	unsigned long fillsz = _wpos - _rpos;

	if (fillsz != 0) {
		unsigned long rpos = _rpos & (_qsize - 1);
		ptr = &_queue[rpos];
		_rpos++;
	} else
		return -1;

	return 0;
}

template<class type>
int queue<type>::_write(type ptr) {
	unsigned long leftsz = _qsize - _wpos + _rpos;

	if (leftsz != 0) {
		unsigned long wpos = _wpos & (_qsize - 1);
		_queue[wpos] = ptr;
		_wpos++;
	} else
		return -1;

	return 0;
}

template<class type>
int queue<type>::get_waittm(struct timespec *tw, int elapse) {
	::clock_gettime(CLOCK_REALTIME, tw);
	tw->tv_sec += (time_t) (elapse / 1000);
	tw->tv_nsec += ((long) (elapse % 1000)) * 1000 * 1000;
	if (tw->tv_nsec >= 1000 * 1000 * 1000) {
		tw->tv_sec += 1;
		tw->tv_nsec -= 1000 * 1000 * 1000;
	}

	return 0;
}

template<class type>
unsigned long queue<type>::roundup_power_of_two(unsigned long val) {
	/*val is the power of 2*/
	if ((val & (val - 1)) == 0)
		return val;

	/*expand val to power of 2*/
	unsigned long maxulong = (unsigned long) ((unsigned long) ~0);
	unsigned long andv = ~(maxulong & (maxulong >> 1));
	while ((andv & val) == 0)
		andv = andv >> 1;

	return andv << 1;
}
END_SERVICE_NS
