#include "Repository.h"
#include "../models/Employee.h"
#include "RepositoryHub.h"

#ifndef EMPLOYEE_REPOSITORY_H
#define EMPLOYEE_REPOSITORY_H

using namespace std;


class EmployeeRepository : public Repository<Employee> {

private:
	JobRepository* jobRepository;

public:
	EmployeeRepository() {}

	EmployeeRepository(DbConnector dbConnector, JobRepository* jobRepository) {
		this->dbConnector = dbConnector;
		this->jobRepository = jobRepository;
	}

	void checkTableExists() {
		this->dbConnector.querySql("create table if not exists employees ( id serial primary key, firstname varchar(30) not null, patronymic varchar(30) not null, lastname varchar(30) not null, date_of_birth date not null, residential_address varchar not null, job_id integer references jobs (id) on delete cascade not null, salary numeric not null );");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(Employee route) {
		if (!dbConnector.isConnected()) {
			if (route.id == 0) {
				route.id = std::time(0);
				models.push_back(route);
			}
			else {
				vector<Employee> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == route.id) {
						newModels.push_back(route);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}


			return true;
		}

		models.push_back(route);
		return route.save(dbConnector.getDBC());
	}
	vector<Employee> loadModels();
	vector<Employee> loadModels(int offset);
	vector<Employee> loadModels(string search);
	vector<Employee> loadModels(string search, int offset);

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<Employee> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif