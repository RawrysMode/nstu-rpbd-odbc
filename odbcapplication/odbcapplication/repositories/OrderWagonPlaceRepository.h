#include "Repository.h"
#include "../models/OrderWagonPlace.h"
#include "RepositoryHub.h"

#ifndef ORDER_WAGON_PLACE_REPOSITORY_H
#define ORDER_WAGON_PLACE_REPOSITORY_H

using namespace std;

class OrderWagonPlaceRepository : public Repository<OrderWagonPlace> {

private:
	OrderRepository* orderRepository;

public:
	OrderWagonPlaceRepository() {}

	OrderWagonPlaceRepository(DbConnector dbConnector, OrderRepository* orderRepository) {
		this->dbConnector = dbConnector;
		this->orderRepository = orderRepository;
	}

	void checkTableExists() {
		this->dbConnector.querySql("create table order_wagon_places(id serial primary key, order_id integer references orders(id) on delete cascade not null, space_number integer not null, size varchar(15) not null, weight varchar(15) not null, insurance_cost integer not null);");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(OrderWagonPlace orderWagonPlace) {
		if (!dbConnector.isConnected()) {
			if (orderWagonPlace.id == 0) {
				orderWagonPlace.id = std::time(0);
				models.push_back(orderWagonPlace);
			}
			else {
				vector<OrderWagonPlace> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == orderWagonPlace.id) {
						newModels.push_back(orderWagonPlace);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}

			return true;
		}

		models.push_back(orderWagonPlace);
		return orderWagonPlace.save(dbConnector.getDBC());
	}

	vector<OrderWagonPlace> loadModels();
	vector<OrderWagonPlace> loadModels(int offset);
	vector<OrderWagonPlace> loadModels(string search);
	vector<OrderWagonPlace> loadModels(string search, int offset);

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<OrderWagonPlace> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif