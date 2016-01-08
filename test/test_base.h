/*
 * test_base.h
 *
 *  Created on: 2016年1月4日
 *      Author: wrb00_000
 */

#ifndef TEST_BASE_H_
#define TEST_BASE_H_

class test_base {
public:
	virtual int run_test() = 0;

public:
	test_base(){}
	virtual ~test_base(){}
};

#endif /* TEST_BASE_H_ */
