#ifndef ORDER_H
#define ORDER_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "ActiveRecord.h"

#include "Employee.h"
#include "Route.h"
#include "Client.h"

using namespace std;

class Order : public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	Employee employee;
	Route route;
	Client client;

	string orderDate, shippingDate, invoiceNumber;
	int employeeId, routeId, clientId;
	int wagonNumber, shippingCost;

	Order();
	Order(int id);

	bool remove(HDBC hDBC) override;
	void setEmployee(Employee employee);
	void setRoute(Route route);
	void setClient(Client client);

	friend bool operator==(const Order lhs, const Order rhs) {
		return lhs.employeeId == rhs.employeeId &&
			lhs.routeId == rhs.routeId &&
			lhs.clientId == rhs.clientId &&
			lhs.wagonNumber == rhs.wagonNumber &&
			lhs.shippingCost == rhs.shippingCost &&
			lhs.orderDate == rhs.orderDate &&
			lhs.shippingDate == rhs.shippingDate &&
			lhs.invoiceNumber == rhs.invoiceNumber;
	}

	std::string toString() {
		char s[3000];
		sprintf_s(s, 3000, "",
			client.toStringCN().c_str(), employee.toStringFPL().c_str(), orderDate.c_str(), route.toString().c_str(), wagonNumber, shippingDate.c_str(), shippingCost, invoiceNumber.c_str());
		return string(s);
	}
};

#endif
