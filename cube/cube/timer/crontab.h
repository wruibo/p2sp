#ifndef _CUBE_TIMER_CRONTAB_H
#define _CUBE_TIMER_CRONTAB_H
#include <list>
#include "cube/timer/task.h"
#include "cube/thread/thread.h"
BEGIN_TIMER_NS
/*data structure for cron task*/
class cron_t {
public:
	cron_t(int id, task *t, int min, int hour, int mday, int mon, int wday):
	_id(id),_task(t),_min(min),_hour(hour),_mday(mday),_mon(mon),_wday(wday)
	{
	}

	~cron_t()
	{
		_task = 0;
	}

	inline int id()
	{
		return _id;
	}

	bool timeup(int curr_min, int curr_hour, int curr_mday, int curr_mon, int curr_wday)
	{
		static const int ANY_TIME = -1;
		return ((_mon==ANY_TIME)||(_mon==curr_mon)) && ((_wday==ANY_TIME)||(_wday==curr_wday)) &&
		((_mday==ANY_TIME)||(_mday==curr_mday)) && ((_hour==ANY_TIME)||(_hour==curr_hour)) &&
		((_min==ANY_TIME)||(_min==curr_min));
	}

	void run()
	{
		if(_task != 0)
		_task->run();
	}
private:
	int _id; //id of the cron task
	task *_task;//cron task

	int _min;//minute of hour
	int _hour;//hour of day
	int _mday;//day of month
	int _mon;//month of year
	int _wday;//day of week
};

/*cron table class, singleton mode*/
class crontab : public cube::thread::runnable
{
public:
	crontab(void);
	virtual ~crontab(void);

	/*
	 *	initialize the cron table object
	 *
	 *return:
	 *	0--success, <0--failed
	 */
	int init();

	/*
	 *	schedule a cron task, if the time item value set with -1 then the
	 *time item is ignored.
	 *
	 *@param t task to schedule
	 *@param min minute value of an hour for running the task, 0~59
	 *@param hour hour value of a day for running the task, 0~23
	 *@param mday day value of a month for running the task, 1~31
	 *@param mon month value of a year for running the task, 1~12
	 *@param wday day value of a week for running the task, 1~7
	 *
	 *return:
	 *	task id in the cron tables or <0 indicate an error
	 */
	int schedule(task *t, int min=-1, int hour=-1, int mday=-1, int mon=-1, int wday=-1);

	/*
	 *	remove a task from cron table by task id
	 *@param tid task id return by @schedule method
	 *return:
	 *	0--success, <0--failed, >0--task not exist
	 */
	int remove(int tid);

	/*
	 *	destroy the cron table object
	 *return:
	 *	0--success, <0--failed
	 */
	int destroy();

private:
	/*clear the crontab items*/
	int clear();

	/*wait for a while*/
	int wait();

	/*check for the timeout of the crontab item*/
	int check();

	/*loop for check the cron task item & run the item*/
	virtual void loop();

private:
	//cron task items
	std::list<cron_t> _items;
	//mutex for the cron task items
	cube::thread::mutex _mutex;

	//thread for the crontab
	cube::thread::thread _thread;

	//unique global cron task id
	static int _global_cron_id;
};

static int crontab::_global_cron_id = 0;
crontab::crontab(void) {
}

crontab::~crontab(void) {

}

int crontab::init() {
	/*start cron task thread*/
	int err = _thread.start(this);
	if (err != 0)
	return -1;

	return 0;
}

int crontab::schedule(task *t, int min/* =-1 */, int hour/* =-1 */,
		int mday/* =-1 */, int mon/* =-1 */, int wday/* =-1 */) {
	cube::thread::scope_lock<cube::thread::mutex> lock(_mutex);
	/*increase the global crontab item id*/
	_global_cron_id++;

	/*generate the cron item*/
	_items.push_back(cron_t(_global_cron_id, t, min, hour, mday, mon, wday));

	return _global_cron_id;
}

int crontab::remove(int tid) {
	cube::thread::scope_lock<cube::thread::mutex> lock(_mutex);
	std::list<cron_t>::iterator iter = _items.begin(), iterend = _items.end();
	while (iter != iterend) {
		if ((*iter).id() == tid) {
			_items.erase(iter++);
			break;
		} else
		iter++;
	}

	return 0;
}

int crontab::destroy() {
	/*stop the crontab thread first*/
	_thread.stop();

	/*clear the crontab items*/
	this->clear();

	return 0;
}

int crontab::clear() {
	cube::thread::scope_lock<cube::thread::mutex> lock(_mutex);
	_items.clear();

	return 0;
}

int crontab::wait() {
	/*get the current time*/
	time_t now = ::time(0);
	struct tm *t = localtime(&now);

	/*wait for the first second of a minute*/
	cube::thread::sleep(1000 * (60 - t->tm_sec));
	return 0;
}

int crontab::check() {
	cube::thread::scope_lock<cube::thread::mutex> lock(_mutex);
	/*get the current time*/
	time_t now = ::time(0);
	struct tm *t = ::localtime(&now);

	/*scan all the cron items for the triggered*/
	std::list<cron_t>::iterator iter = _items.begin(), iterend = _items.end();
	while (iter != iterend) {
		if ((*iter).timeup(t->tm_min, t->tm_hour, t->tm_mday, t->tm_mon + 1,
						t->tm_wday + 1))
		(*iter).run();
		iter++;
	}

	return 0;
}

void crontab::loop() {
	//wait for a complete minute
	this->wait();
	//check all the cron items, run the triggered item task
	this->check();
}
END_TIMER_NS
#endif

