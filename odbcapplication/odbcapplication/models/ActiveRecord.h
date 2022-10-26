#ifndef ACTIVE_RECORD_H
#define ACTIVE_RECORD_H

#include<windows.h>
#include<sql.h>
#include<sqlext.h>
#include<iostream>
#include <vector>
#include "ActiveRecordException.h"
#include "tchar.h"

class ActiveRecord {
protected:
	virtual int insert(HDBC hDBC) {
		return 0;
	};
	virtual bool update(HDBC hDBC) {
		return false;
	};
	void checkRetCodeException(RETCODE retCode);
	void checkRetCodeException(RETCODE retCode, std::string comment);

public:
	long long id;
	bool save(HDBC hDBC);
	
	virtual bool remove(HDBC hDBC) {
		return false;
	}
	
	virtual std::string toString() {
		return "Empty Active Record";
	}
};

#endif