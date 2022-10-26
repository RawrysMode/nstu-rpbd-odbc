#ifndef CITY_H
#define CITY_H

#include<windows.h>
#include<sql.h>
#include<sqlext.h>
#include<iostream>
#include "ActiveRecord.h"

using namespace std;

class City: public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	string name;
	City();
	City(long long id);
	City(long long id, string name);
	City(string name);

	bool remove(HDBC hDBC) override;
	City load(HDBC hDBC);

	friend bool operator==(const City& lhs, const City& rhs) {
		return lhs.id == rhs.id && lhs.name == rhs.name;
	}

	std::string toString() {
		return name;
	}
};

#endif