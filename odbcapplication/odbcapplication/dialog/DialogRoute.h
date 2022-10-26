#ifndef DIALOG_ROUTE_H
#define DIALOG_ROUTE_H

#include <iostream>
#include <vector>
#include "DialogRouteType.h"
#include "DialogForm.h"

using namespace std;

class DialogRoute {

public:
	DialogRouteType type;
	string modelName = "";
	string search = "";
	string errorMessage = "";
	DialogForm dialogForm = DialogForm();
	int pickedModelId = 0;

	int offset = 0;

	DialogRoute(DialogRouteType type) {
		this->type = type;
	}

	DialogRoute(DialogRouteType type, string modelName) {
		this->type = type;
		this->modelName = modelName;
	}

	DialogRoute(DialogRouteType type, string modelName, int offset) {
		this->type = type;
		this->modelName = modelName;
		this->offset = offset;
	}

	DialogRoute(DialogRouteType type, string modelName, int offset, string search) {
		this->type = type;
		this->modelName = modelName;
		this->offset = offset;
		this->search = search;
	}
};

#endif