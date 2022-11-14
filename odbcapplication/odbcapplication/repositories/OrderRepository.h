#include "Repository.h"
#include "../models/Order.h"
#include "RepositoryHub.h"

#ifndef ORDER_REPOSITORY_H
#define ORDER_REPOSITORY_H

using namespace std;

class OrderRepository : public Repository<Order> {

private:
	ClientRepository* clientRepository;
	EmployeeRepository* employeeRepository;
	RouteRepository* routeRepository;
	

public:
	OrderRepository() {}

	OrderRepository(DbConnector dbConnector, ClientRepository* clientRepository, EmployeeRepository* employeeRepository, RouteRepository* routeRepository) {
		this->dbConnector = dbConnector;
		this->clientRepository = clientRepository;
		this->employeeRepository = employeeRepository;
		this->routeRepository = routeRepository;
	}

	void checkTableExists() {
		this->dbConnector.querySql("create table if not exists orders ( id serial primary key, client_id integer references clients (id) on delete cascade not null, employee_id integer references employees (id) on delete cascade not null, order_date timestamp without time zone not null, route_id integer references routes (id) on delete cascade not null, wagon_number integer not null, shipping_date date not null, shipping_cost numeric not null, nvc varchar not null )");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(Order order) {
		if (!dbConnector.isConnected()) {
			if (order.id == 0) {
				order.id = std::time(0);
				models.push_back(order);
			}
			else {
				vector<Order> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == order.id) {
						newModels.push_back(order);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}

			return true;
		}

		models.push_back(order);
		return order.save(dbConnector.getDBC());
	}

	vector<Order> loadModels();
	vector<Order> loadModels(int offset);
	vector<Order> loadModels(string search);
	vector<Order> loadModels(string search, int offset);

	Order loadModelById(int id) {
		if (!this->dbConnector.isConnected()) {
			for (int i = 0; i < models.size(); i++) {
				if (models[i].id == id) {
					return models[i];
				}
			}

			return Order(id);
		}

		return Order(id).load(this->dbConnector.getDBC());
	}

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<Order> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif