#ifndef CITY_REPOSITORY_H
#define CITY_REPOSITORY_H

#include "Repository.h"
#include "../models/City.h"
using namespace std;


class CityRepository : public Repository<City> {

public:
	CityRepository() {}
	CityRepository(DbConnector dbConnector) {
		this->dbConnector = dbConnector;
	}

	void checkTableExists() {
		this->dbConnector.querySql("CREATE TABLE IF NOT EXISTS cities (id serial primary key, city_name varchar(25) not null);");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(City city) {
		if (!dbConnector.isConnected()) {
			if (city.id == 0) {
				city.id = std::time(0);
				models.push_back(city);
			} else {
				vector<City> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == city.id) {
						newModels.push_back(city);
					} else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}
			

			return true;
		}

		models.push_back(city);
		return city.save(dbConnector.getDBC());
	}
	vector<City> loadModels();
	vector<City> loadModels(int offset);
	vector<City> loadModels(string search);
	vector<City> loadModels(string search, int offset);

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<City> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif