#ifndef JOBS_H
#define JOBS_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "ActiveRecord.h"

using namespace std;

class Job : public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	string jobTitle;

	Job();
	Job(long long id);
	Job(long long id, string name);

	bool remove(HDBC hDBC) override;
	Job load(HDBC hDBC);

	friend bool operator==(const Job& lhs, const Job& rhs) {
		return lhs.id == rhs.id && lhs.jobTitle == rhs.jobTitle;
	}

	std::string toString() {
		return jobTitle;
	}
};

#endif
