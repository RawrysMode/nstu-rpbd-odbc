#ifndef JOB_REPOSITORY_H
#define JOB_REPOSITORY_H

#include "Repository.h"
#include "../models/Job.h"
using namespace std;


class JobRepository : public Repository<Job> {

public:
	JobRepository() {}
	JobRepository(DbConnector dbConnector) {
		this->dbConnector = dbConnector;
	}

	void checkTableExists() {
		this->dbConnector.querySql("CREATE TABLE IF NOT EXISTS jobs (id serial primary key, job_title not null);");
	}

	int loadModelsCount();
	int loadModelsCount(string search);

	bool saveModel(Job job) {
		if (!dbConnector.isConnected()) {
			if (job.id == 0) {
				job.id = std::time(0);
				models.push_back(job);
			}
			else {
				vector<Job> newModels = {};
				for (int i = 0; i < models.size(); i++) {
					if (models[i].id == job.id) {
						newModels.push_back(job);
					}
					else {
						newModels.push_back(models[i]);
					}
				}

				models = newModels;
			}

			return true;
		}

		models.push_back(job);
		return job.save(dbConnector.getDBC());
	}
	vector<Job> loadModels();
	vector<Job> loadModels(int offset);
	vector<Job> loadModels(string search);
	vector<Job> loadModels(string search, int offset);

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* modelPageSize, string search, vector<string>* modelLabels) {
		*totalModelsCount = loadModelsCount(search);
		*modelPageSize = pageSize;
		vector<Job> models = loadModels(search, route.offset * *modelPageSize);
		modelLabels->clear();
		for (int i = 0; i < models.size(); i++) {
			modelLabels->push_back(models[i].toString());
		}
	}
};

#endif