#include "Repository.h"
#include "../models/BankDetails.h"
#include "RepositoryHub.h"

#ifndef BANK_DETAILS_REPOSITORY_H
#define BANK_DETAILS_REPOSITORY_H

using namespace std;


class BankDetailsRepository : public Repository<BankDetails> {

private:
	ClientRepository* clientRepository;
	CityRepository* cityRepository;

public:
	BankDetailsRepository() {}

	BankDetailsRepository(DbConnector dbConnector, ClientRepository* clientRepository, CityRepository* cityRepository) {
		this->dbConnector = dbConnector;
		this->clientRepository = clientRepository;
		this->cityRepository = cityRepository;
	}

	void checkTableExists() {
		this->dbConnector.querySql("create table if not exists bank_details (id serial primary key, client_id integer references clients (id) on delete cascade not null, bank_name varchar not null, city_id integer references cities (id) on delete cascade not null, tin varchar(10) not null constraint _invalid_tin_ check ( length(tin) = 10 ) unique, bank_account varchar(20) not null constraint _invalid_bank_account_ check ( length(bank_account) = 20 ) unique);");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(BankDetails bankDetails) {
		if (!dbConnector.isConnected()) {
			if (bankDetails.id == 0) {
				bankDetails.id = std::time(0);
				models.push_back(bankDetails);
			}
			else {
				vector<BankDetails> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == bankDetails.id) {
						newModels.push_back(bankDetails);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}

			return true;
		}

		models.push_back(bankDetails);
		return bankDetails.save(dbConnector.getDBC());
	}

	vector<BankDetails> loadModels();
	vector<BankDetails> loadModels(int offset);
	vector<BankDetails> loadModels(string search);
	vector<BankDetails> loadModels(string search, int offset);

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<BankDetails> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif