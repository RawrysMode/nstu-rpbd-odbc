#ifndef CLIENT_REPOSITORY_H
#define CLIENT_REPOSITORY_H

#include "Repository.h"
#include "../models/Client.h"
using namespace std;


class ClientRepository : public Repository<Client> {

public:
	ClientRepository() {}
	ClientRepository(DbConnector dbConnector) {
		this->dbConnector = dbConnector;
	}

	void checkTableExists() {
		this->dbConnector.querySql("CREATE TABLE IF NOT EXISTS clients (id serial primary key, company_name varchar not null, postal_address varchar not null, phone_number varchar not null, fax_number varchar not null, email varchar not null);");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(Client client) {
		if (!dbConnector.isConnected()) {
			if (client.id == 0) {
				client.id = std::time(0);
				models.push_back(client);
			}
			else {
				vector<Client> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == client.id) {
						newModels.push_back(client);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}


			return true;
		}

		models.push_back(client);
		return client.save(dbConnector.getDBC());
	}
	vector<Client> loadModels();
	vector<Client> loadModels(int offset);
	vector<Client> loadModels(string search);
	vector<Client> loadModels(string search, int offset);

	Client loadModelById(int id) {
		if (!this->dbConnector.isConnected()) {
			for (int i = 0; i < models.size(); i++) {
				if (models[i].id == id) {
					return models[i];
				}
			}

			return Client(id);
		}

		return Client(id).load(this->dbConnector.getDBC());
	}

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<Client> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif