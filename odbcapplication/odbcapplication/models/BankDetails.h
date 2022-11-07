#ifndef BANK_DETAILS_H
#define BANK_DETAILS_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "ActiveRecord.h"
#include "Client.h"
#include "City.h"

using namespace std;

class BankDetails : public ActiveRecord {

private:
	int insert(HDBC hDBC) override;
	bool update(HDBC hDBC) override;

public:
	Client companyName;
	City cityName;
	string bankName, taxpayerIN, bankAccount;
	int cityId, companyId;

	BankDetails();
	BankDetails(int id);

	bool remove(HDBC hDBC) override;
	void setCompanyName(Client companyName);
	void setCityName(City cityName);

	friend bool operator==(const BankDetails lhs, const BankDetails rhs) {
		return lhs.companyId == rhs.companyId &&
			lhs.cityId == rhs.cityId &&
			lhs.bankName == rhs.bankName &&
			lhs.taxpayerIN == rhs.taxpayerIN &&
			lhs.bankAccount == rhs.bankAccount;
	}

	std::string toString() {
		char s[6000];
		sprintf_s(s, 6000, "Company Name: %s, Bank Name: %s, City Name: %s, Taxpayer Identification Number: %s, Bank Account: %s",
			companyName.toString().c_str(), bankName.c_str(), cityName.toString().c_str(), taxpayerIN.c_str(), bankAccount.c_str());
		return string(s);
	}
};

#endif
