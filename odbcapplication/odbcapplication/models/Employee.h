#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "ActiveRecord.h"
#include "Job.h"

using namespace std;

class Employee : public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	string firstname, patronymic, lastname;
	string dateOfBirth;
	string residentialAddress;
	int jobId;
	int salary;
	Job jobTitle;

	Employee();
	Employee(int id);

	bool remove(HDBC hDBC) override;
	void setJobTitle(Job jobTitle);

	friend bool operator==(const Employee& lhs, const Employee& rhs) {
		return lhs.id == rhs.id &&
			lhs.firstname == rhs.firstname &&
			lhs.patronymic == rhs.patronymic &&
			lhs.lastname == rhs.lastname &&
			lhs.dateOfBirth == rhs.dateOfBirth &&
			lhs.residentialAddress == rhs.residentialAddress &&
			lhs.salary == rhs.salary &&
			lhs.jobId == rhs.jobId;
	}

	std::string toString() {
		char s[6000];
		sprintf_s(s, 6000, "First Name: %s, Patronymic: %s, Last Name: %s, Date Of Birth: %s, Residential Address: %s, Job: %s, Salary: %d",
			firstname.c_str(), patronymic.c_str(), lastname.c_str(), dateOfBirth.c_str(), residentialAddress.c_str(), jobTitle.toString().c_str(), salary);
		return string(s);
	}
};

#endif