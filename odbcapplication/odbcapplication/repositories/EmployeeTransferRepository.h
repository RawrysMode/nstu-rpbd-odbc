#include "Repository.h"
#include "../models/EmployeeTransfer.h"
#include "RepositoryHub.h"

#ifndef EMPLOYEE_TRANSFER_REPOSITORY_H
#define EMPLOYEE_TRANSFER_REPOSITORY_H

using namespace std;

class EmployeeTransferRepository : public Repository<EmployeeTransfer> {

private:
	EmployeeRepository* employeeRepository;
	JobRepository* jobRepository;

public:
	EmployeeTransferRepository() {}

	EmployeeTransferRepository(DbConnector dbConnector, EmployeeRepository* employeeRepository, JobRepository* jobRepository) {
		this->dbConnector = dbConnector;
		this->employeeRepository = employeeRepository;
		this->jobRepository = jobRepository;
	}

	void checkTableExists() {
		this->dbConnector.querySql("create table if not exists employee_transfers ( id serial primary key, employee_id integer references employees (id) on delete cascade not null, transfer_reason varchar not null, old_job_id integer references jobs (id) on delete cascade not null, new_job_id integer references jobs (id) on delete cascade not null, order_number integer not null, order_date date not null );");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(EmployeeTransfer employeeTransfer) {
		if (!dbConnector.isConnected()) {
			if (employeeTransfer.id == 0) {
				employeeTransfer.id = std::time(0);
				models.push_back(employeeTransfer);
			}
			else {
				vector<EmployeeTransfer> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == employeeTransfer.id) {
						newModels.push_back(employeeTransfer);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}

			return true;
		}

		models.push_back(employeeTransfer);
		return employeeTransfer.save(dbConnector.getDBC());
	}

	vector<EmployeeTransfer> loadModels();
	vector<EmployeeTransfer> loadModels(int offset);
	vector<EmployeeTransfer> loadModels(string search);
	vector<EmployeeTransfer> loadModels(string search, int offset);

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<EmployeeTransfer> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif