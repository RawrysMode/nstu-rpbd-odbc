#ifndef ACTIVE_RECORD_EXCEPTION_H
#define ACTIVE_RECORD_EXCEPTION_H

#include<iostream>
#include<sql.h>
#include<sqlext.h>

class ActiveRecordException {

public:
	std::string message;
	RETCODE retCode;

	ActiveRecordException(RETCODE retCode, std::string message) {
		this->message = message;
		this->retCode = retCode;
	}
};

#endif
