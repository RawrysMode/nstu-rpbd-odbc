#ifndef EMPLOYEE_TRANSFER_H
#define EMPLOYEE_TRANSFER_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "ActiveRecord.h"
#include "Job.h"
#include "Employee.h"

using namespace std;

class EmployeeTransfer : public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	Job oldJobTitle, newJobTitle;
	Employee employee;
	string transferReason, orderDate;
	int employeeId, oldJobId, newJobId, orderNumber;

	EmployeeTransfer();
	EmployeeTransfer(int id);

	bool remove(HDBC hDBC) override;
	void setEmployee(Employee employee);
	void setOldJob(Job oldJobTitle);
	void setNewJob(Job newJobTitle);

	friend bool operator==(const EmployeeTransfer lhs, const EmployeeTransfer rhs) {
		return lhs.employeeId == rhs.employeeId &&
			lhs.transferReason == rhs.transferReason &&
			lhs.oldJobId == rhs.oldJobId &&
			lhs.newJobId == rhs.newJobId &&
			lhs.orderNumber == rhs.orderNumber &&
			lhs.orderDate == rhs.orderDate;
	}

	std::string toString() {
		char s[3000];
		sprintf_s(s, 3000, "%s, Transfer Reason: %s, Old Job Title: %s, New Job Title: %s, Order Number: %d, Order Date: %s",
			employee.toStringFPL().c_str(), transferReason.c_str(), oldJobTitle.toString().c_str(), newJobTitle.toString().c_str(), &orderNumber, orderDate.c_str());
		return string(s);
	}
};

#endif
