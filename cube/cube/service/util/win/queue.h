/*
 * queue.h
 *
 *  Created on: 2015年11月17日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_WIN_QUEUE_H_
#define CUBE_SERVICE_UTIL_WIN_QUEUE_H_
#include <Windows.h>

namespace cube {
namespace service {
/*
 *	read write queue which is thread safe for only one reader & writer.
 *no lock used, it has high performance. it work best if make sure the
 *reader is faster than the writer
 *
 *	!!notes!!:
 *	please adjust "queue size" for a good performance under different
 *platform with different hardware, like: CPU.
 */
template<class type>
class queue {
public:
	queue();
	queue(unsigned long qsz);
	~queue();

	/*
	 *	initialize the queue with queue size @qsz
	 *@param qsz: queue size
	 *return:
	 *	0--success, -1--failed
	 */
	int init(unsigned long qsz);

	/*
	 *	read a value from queue in block mode until read success
	 *@param ptr: in & out pointer to the read value
	 *@param waittm: million seconds for read waiting
	 *return:
	 *	0--always success, -1--failed
	 */
	int read(type *ptr, int waittm = -1);

	/*
	 *	write a value to the queue in block mode until write success
	 *@param ptr: pointer value to write
	 *@param waittm: million seconds for write waiting
	 *return:
	 *	0--always success, -1--failed
	 */
	int write(type ptr, int waittm = -1);

	/*
	 *	read a value from queue in non-block mode
	 *@param ptr: point to value if there is readable value in the queue
	 *return:
	 *	0--success, -1--no value has read, must wait
	 */
	int _read(type *ptr);

	/*
	 *	write a value into the queue in non-block mode
	 *@param ptr: value to write
	 *return:
	 *	0--success, -1--no space can be used, must wait
	 */
	int _write(type ptr);

public:
	/*
	 *	get the elements number in the queue
	 */
	unsigned int size() {
		volatile unsigned int num = _wpos - _rpos;
		return num;
	}

private:
	/*
	 *	expends a @val to power of 2. for example: 3->4, 7->8, 12->16, 21->32
	 */
	unsigned long roundup_power_of_two(unsigned long val);

private:
	//queue size
	unsigned long _qsize;

	//queue array
	type* _queue;

	//wait flag for read operation
	volatile long _rwflag;

	//wait flag for write operation
	volatile long _wwflag;

	//current read position of queue array
	volatile unsigned long _rpos;

	//next write position of queue array
	volatile unsigned long _wpos;

	//wait condition when read position catch the write position
	HANDLE _cond1;
	//wait condition when write position catch the read position
	HANDLE _cond2;
};

template<class type>
queue<type>::queue() :
		_qsize(0), _queue(NULL), _rwflag(0), _wwflag(0), _rpos((unsigned long) -1), _wpos((unsigned long) -1), _cond1(0), _cond2(0) {

}

template<class type>
queue<type>::queue(unsigned long qsz) {
	/*initial value of queue*/
	_qsize = roundup_power_of_two(qsz);

	/*allocate the queue buffer*/
	_queue = new void*[_qsize];

	/*initialize the read&write wait flag*/
	_rwflag = 1;
	_wwflag = 0;

	/*initialize the read&write position*/
	_rpos = 0;
	_wpos = 0;

	/*initialize the condition variable*/
	_cond1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_cond2 = CreateEvent(NULL, FALSE, FALSE, NULL);
}

template<class type>
queue<type>::~queue() {
	if (_queue != NULL)
		delete[] _queue;
	_queue = 0;

	CloseHandle(_cond1);
	CloseHandle(_cond2);
}

template<class type>
int queue<type>::init(unsigned long qsz) {
	/*initial value of queue*/
	_qsize = roundup_power_of_two(qsz);

	/*allocate the queue buffer*/
	_queue = new type [_qsize];
	if (_queue == 0)
		return -1;

	/*initialize the read&write wait flag*/
	_rwflag = 1;
	_wwflag = 0;

	/*initialize the read&write position*/
	_rpos = 0;
	_wpos = 0;

	/*initialize the condition variable*/
	_cond1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_cond2 = CreateEvent(NULL, FALSE, FALSE, NULL);

	return 0;
}

template<class type>
int queue<type>::read(type *ptr, int waittm/* =-1 */) {
	DWORD wtm = INFINITE;
	if (!(waittm < 0))
		wtm = waittm;

	int err = _read(ptr);
	while (err != 0) {
		_rwflag = 1;
		if (_wwflag == 1) {
			_wwflag = 0;
			SetEvent(_cond2);
		}

		DWORD res = WaitForSingleObject(_cond1, wtm);
		if (!(waittm < 0) && res == WAIT_TIMEOUT) {
			err = _read(ptr);
			break;
		}

		err = _read(ptr);
	}

	if (_wwflag == 1) {
		_wwflag = 0;
		SetEvent(_cond2);
	}

	if (err != 0)
		return -1;

	return 0;
}

template<class type>
int queue<type>::write(type ptr, int waittm/* =-1 */) {
	DWORD wtm = INFINITE;
	if (!(waittm < 0))
		wtm = waittm;

	int err = _write(ptr);
	while (err != 0) {
		_wwflag = 1;
		if (_rwflag == 1) {
			_rwflag = 0;
			SetEvent(_cond1);
		}

		DWORD res = WaitForSingleObject(_cond2, wtm);
		if (!(waittm < 0) && res == WAIT_TIMEOUT) {
			err = _write(ptr);
			break;
		}

		err = _write(ptr);
	}

	if (_rwflag == 1) {
		_rwflag = 0;
		SetEvent(_cond1);
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

}
}
#endif /* CUBE_SERVICE_UTIL_WIN_QUEUE_H_ */
