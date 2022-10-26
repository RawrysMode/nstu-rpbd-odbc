#ifndef REPOSITORY_H
#define REPOSITORY_H

#include<windows.h>
#include<sql.h>
#include<sqlext.h>
#include<iostream>
#include <vector>
#include "tchar.h"
#include "../DbConnector.h"
#include "../dialog/DialogRoute.h"

using namespace std;

template <typename T>
class Repository {
protected:
	vector<T> models = {};
	DbConnector dbConnector;

public:
	int pageSize = 5;
	Repository() {}
	Repository(DbConnector dbConnector) {
		this->dbConnector = dbConnector;
	}

	void checkTableExists();
	int loadModelsCount();
	int loadModelsCount(string search);
	bool saveModel(T model) {
		models.push_back(model);
		return true;
	}
	void deleteModel(T model) {
		if (dbConnector.isConnected()) {
			model.remove(dbConnector.getDBC());
		}
		
		vector<T> newModels = {};
		for (int i = 0; i < models.size(); i++) {
			if ((models[i] == model) == false) {
				newModels.push_back(models[i]);
			}
		}
		models = newModels;
	}
	vector<T> loadModels();
	vector<T> loadModels(int offset);
	vector<T> loadModels(string search);
	vector<T> loadModels(string search, int offset);
};

#endif
