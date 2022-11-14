#ifndef ORDER_WAGON_PLACE_H
#define ORDER_WAGON_PLACE_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "ActiveRecord.h"

#include "Order.h"
#include "Client.h"

using namespace std;

class OrderWagonPlace : public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	Order order;
	Client client;

	int clientId, orderId, spaceNumber, insuranceCost;
	string size, weight;

	OrderWagonPlace();
	OrderWagonPlace(int id);

	bool remove(HDBC hDBC) override;
	//void setClient(Client client);
	void setOrder(Order order);

	friend bool operator==(const OrderWagonPlace lhs, const OrderWagonPlace rhs) {
		return
			lhs.clientId == rhs.clientId &&
			lhs.orderId == rhs.orderId &&
			lhs.spaceNumber == rhs.spaceNumber &&
			lhs.insuranceCost == rhs.insuranceCost &&
			lhs.size == rhs.size &&
			lhs.weight == rhs.weight;
	}

	std::string toString() {
		char s[6000];
		sprintf_s(s, 6000, "%s, %s, Space Number: %d, Size: %s, Weight: %s, Insurance Cost: %d", client.toStringCN().c_str(), order.toStringWSS().c_str(), spaceNumber, size.c_str(), weight.c_str(), insuranceCost); // companyName, order.wagonNumber, order.shippingDate, order.shippingCost, spaceNumber, size, weight, insuranceCost
		return string(s);
	}
};

#endif
