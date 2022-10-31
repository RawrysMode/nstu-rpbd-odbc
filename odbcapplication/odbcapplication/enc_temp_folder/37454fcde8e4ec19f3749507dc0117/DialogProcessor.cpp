#include "DialogProcessor.h"
#include "../repositories/repositoryHub.h"
#include <iostream>
#include <string>
using namespace std;

void DialogProcessor::popRoute() {
	routes.pop_back();
}

void DialogProcessor::pushRoute(DialogRoute route) {
	routes.push_back(route);
}

void DialogProcessor::displayModelLabels(vector<string> modelLabels, int pageSize, int offset, int totalModelsCount, string search) {
	for (int i = 0; i < modelLabels.size(); i++) {
		cout << (i + 1 + offset * pageSize) << ": " << modelLabels[i] << endl;
	}

	if (modelLabels.empty()) {
		cout << "No models found." << endl;
	}

	if (!search.empty()) {
		cout << "-- (search: \"" << search << "\")" << endl;
	}
	else {
		cout << "--" << endl;
	}

	if (!modelLabels.empty() && showSearchAndPagination) {
		int maxPageNumber = totalModelsCount / pageSize + (totalModelsCount % pageSize != 0);
		cout << "Shown page " << (offset + 1) << "/" << maxPageNumber << " (models " << (pageSize * offset + 1) << "-"
			<< min(pageSize * (offset + 1), totalModelsCount) << "/"
			<< totalModelsCount << ")" << endl;
	}
	cout << endl;
}

void DialogProcessor::processRouteEntry(DialogRoute route, string* option) {
	cout << route.modelName;
	cout << "Choose option:" << endl;
	cout << "1 : Cities" << endl;
	cout << "2 : Routes" << endl;
	cout << "3 : Jobs" << endl;
	cout << "4 : Clients" << endl;
	cout << "q : Exit" << endl << endl;

	cout << "> ";
	cin >> *option;

	if (*option == "1") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_VIEW, "Cities"));
	}
	else if (*option == "2") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_VIEW, "Routes"));
	}
	else if (*option == "3") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_VIEW, "Jobs"));
	}
	else if (*option == "4") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_VIEW, "Clients"));
	}
}

void DialogProcessor::processRouteModelView(DialogRoute route, string* option) {
	int totalModelsCount = 0;
	int pageSize = 0;
	vector<string> modelLabels = {};
	repositoryHub->getModelViewRouteData(route, &totalModelsCount, &pageSize, route.search, &modelLabels);

	int maxPageNumber = totalModelsCount / pageSize + (totalModelsCount % pageSize != 0);
	cout << route.modelName << ":" << endl << endl;

	displayModelLabels(modelLabels, pageSize, route.offset, totalModelsCount, route.search);

	if (showSearchAndPagination) {
		if (totalModelsCount > 0) {
			cout << "1-" << maxPageNumber << " : Go to page" << endl;
		}

		cout << "s : Search" << endl;
		if (!route.search.empty()) {
			cout << "x : Clear search" << endl;
		}
	}

	cout << "a : Add model" << endl;
	cout << "e : Edit model" << endl;
	cout << "d : Delete model" << endl;
	cout << "q : Back" << endl << endl;

	cout << "> ";
	cin >> *option;

	char* p;
	long newPage = strtol(option->c_str(), &p, 10);
	if (!*p) {
		popRoute();
		pushRoute(DialogRoute(route.type, route.modelName, min(maxPageNumber, max(1, newPage)) - 1, route.search));
	}

	if (*option == "s") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_SEARCH, route.modelName, route.offset, route.search));
	}

	if (*option == "d") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_DELETE, route.modelName, route.offset, route.search));
	}

	if (*option == "a") {
		DialogRoute addNewModelRoute = DialogRoute(DialogRouteType::MODEL_ADD, route.modelName, route.offset, route.search);
		addNewModelRoute.dialogForm = repositoryHub->getModelAddDialogForm(route);
		pushRoute(addNewModelRoute);
	}

	if (*option == "e") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_EDIT_SELECT, route.modelName, route.offset, route.search));
	}

	if (*option == "x") {
		routes[routes.size() - 1].search = "";
		routes[routes.size() - 1].offset = 0;
	}
}

void DialogProcessor::processRouteModelSearch(DialogRoute route, string* option) {
	int totalModelsCount = 0;
	int pageSize = 0;
	vector<string> modelLabels = {};
	repositoryHub->getModelViewRouteData(route, &totalModelsCount, &pageSize, route.search, &modelLabels);

	cout << route.modelName << ":" << endl << endl;
	displayModelLabels(modelLabels, pageSize, route.offset, totalModelsCount, route.search);

	cout << "Enter search string: ";
	cin >> *option;

	routes[routes.size() - 2].search = *option;
	routes[routes.size() - 2].offset = 0;
	*option = "q";
}

void DialogProcessor::processRouteModelDelete(DialogRoute route, string* option) {
	int totalModelsCount = 0;
	int pageSize = 0;
	vector<string> modelLabels = {};
	repositoryHub->getModelViewRouteData(route, &totalModelsCount, &pageSize, route.search, &modelLabels);

	cout << route.modelName << ":" << endl << endl;
	displayModelLabels(modelLabels, pageSize, route.offset, totalModelsCount, route.search);

	int minNumber = pageSize * route.offset + 1;
	int maxNumber = min(pageSize * (route.offset + 1), totalModelsCount);
	cout << "Enter model number to delete (" << minNumber << "-" << maxNumber << "): ";
	cin >> *option;

	char* p;
	long modelNumber = strtol(option->c_str(), &p, 10);
	if (!*p && modelNumber >= minNumber && modelNumber <= maxNumber) {
		int idx = modelNumber - pageSize * route.offset - 1;
		bool isDeleted = repositoryHub->deleteModelByModelNumber(route, idx);
		if (isDeleted) {
			--totalModelsCount;
			if (totalModelsCount > 0 && totalModelsCount <= pageSize * route.offset) {
				routes[routes.size() - 2].offset--;
			}

			routes[routes.size() - 2].errorMessage = "Model " + modelLabels[idx] + " deleted.";
		}
		else {
			routes[routes.size() - 2].errorMessage = "Couldn't delete model " + modelLabels[idx] + " because it is used by another models.";
		}
	}

	*option = "q";
}

void DialogProcessor::processRouteModelAdd(DialogRoute route, string* option) {
	if (route.dialogForm.isFinished()) {
		if (repositoryHub->saveModel(route)) {
			if (route.dialogForm.modelId == 0) {
				routes[routes.size() - 2].errorMessage = "Model has been added.";
			}
			else {
				routes[routes.size() - 2].errorMessage = "Model has been updated.";
				popRoute();
			}
		}
		else {
			if (route.dialogForm.modelId == 0) {
				routes[routes.size() - 2].errorMessage = "Model is not added.";
			}
			else {
				routes[routes.size() - 2].errorMessage = "Model is not updated.";
				popRoute();
			}
		}

		*option = "q";
	}
	else {
		DialogFormStep step = route.dialogForm.steps[route.dialogForm.currentStepId];

		if (step.type == DialogFormFieldType::FT_STRING) {
			cout << step.title;
			if (route.dialogForm.modelId != 0) {
				cout << " [" << step.sValue << "]: ";
				string text;
				getline(cin, text);
				getline(cin, text);
				if (text.empty()) {
					text = step.sValue;
				}
				routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].sValue = text;
			}
			else {
				cout << ": ";
				cin >> routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].sValue;
			}

			if (routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].sValue.empty()) {
				return;
			}
		}
		else if (step.type == DialogFormFieldType::FT_INT) {
			cout << step.title;
			if (route.dialogForm.modelId != 0) {
				cout << " [" << step.iValue << "]: ";
				string text;
				getline(cin, text);
				getline(cin, text);
				int iValue = 0;

				if (text.empty()) {
					iValue = step.iValue;
				}
				else {
					iValue = atoi(text.c_str());
				}

				routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].iValue = iValue;
			}
			else {
				cout << ": ";
				cin >> routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].iValue;
			}
		}
		else if (step.type == DialogFormFieldType::FT_FLOAT) {
			cout << step.title;
			if (route.dialogForm.modelId != 0) {
				cout << " [" << step.fValue << "]: ";
				string text;
				getline(cin, text);
				getline(cin, text);
				float fValue = 0;

				if (text.empty()) {
					fValue = step.fValue;
				}
				else {
					fValue = atof(text.c_str());
				}

				routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].fValue = fValue;
			}
			else {
				cout << ": ";
				cin >> routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].fValue;
			}
		}
		else if (step.type == DialogFormFieldType::FT_CITY) {
			if (route.pickedModelId != 0) {
				routes[routes.size() - 1].dialogForm.steps[route.dialogForm.currentStepId].iValue = route.pickedModelId;
				routes[routes.size() - 1].pickedModelId = 0;
			}
			else {
				pushRoute(DialogRoute(DialogRouteType::MODEL_PICK, "Cities", 0, ""));
				routes[routes.size() - 1].errorMessage = step.title;
				return;
			}
		}

		routes[routes.size() - 1].dialogForm.nextStep();
	}
}

void DialogProcessor::processRouteModelEdit(DialogRoute route, string* option) {
	return processRouteModelAdd(route, option);
}


void DialogProcessor::processRouteModelPick(DialogRoute route, string* option) {
	int totalModelsCount = 0;
	int pageSize = 0;
	vector<string> modelLabels = {};
	repositoryHub->getModelViewRouteData(route, &totalModelsCount, &pageSize, route.search, &modelLabels);

	cout << route.modelName << ":" << endl << endl;
	displayModelLabels(modelLabels, pageSize, route.offset, totalModelsCount, route.search);

	int minNumber = pageSize * route.offset + 1;
	int maxNumber = min(pageSize * (route.offset + 1), totalModelsCount);
	int maxPageNumber = totalModelsCount / pageSize + (totalModelsCount % pageSize != 0);

	if (showSearchAndPagination) {
		if (route.offset > 0) {
			cout << "w : Previous page" << endl;
		}

		if (route.offset < maxPageNumber - 1) {
			cout << "e : Next page" << endl;
		}

		cout << "s : Search" << endl;
		if (!route.search.empty()) {
			cout << "x : Clear search" << endl;
		}
	}

	cout << "q : Back" << endl << endl;

	cout << "Enter model number (" << minNumber << "-" << maxNumber << "): ";
	cin >> *option;

	char* p;
	long modelNumber = strtol(option->c_str(), &p, 10);
	if (!*p && modelNumber >= minNumber && modelNumber <= maxNumber) {
		int idx = modelNumber - pageSize * route.offset - 1;
		routes[routes.size() - 2].pickedModelId = repositoryHub->getModelId(route, idx, route.search, route.offset);
		popRoute();
	}

	if (*option == "s") {
		pushRoute(DialogRoute(DialogRouteType::MODEL_SEARCH, route.modelName, route.offset, route.search));
	}

	if (*option == "w" && route.offset > 0) {
		routes[routes.size() - 1].offset--;
	}

	if (*option == "e" && route.offset < maxPageNumber) {
		routes[routes.size() - 1].offset++;
	}

	if (*option == "x") {
		routes[routes.size() - 1].search = "";
		routes[routes.size() - 1].offset = 0;
	}

	if (*option == "q") {
		popRoute();
	}
}

void DialogProcessor::processRouteModelEditSelect(DialogRoute route, string* option) {
	int totalModelsCount = 0;
	int pageSize = 0;
	vector<string> modelLabels = {};
	repositoryHub->getModelViewRouteData(route, &totalModelsCount, &pageSize, route.search, &modelLabels);

	cout << route.modelName << ":" << endl << endl;
	displayModelLabels(modelLabels, pageSize, route.offset, totalModelsCount, route.search);

	int minNumber = pageSize * route.offset + 1;
	int maxNumber = min(pageSize * (route.offset + 1), totalModelsCount);
	cout << "Enter model number to edit (" << minNumber << "-" << maxNumber << "): ";
	cin >> *option;

	char* p;
	long modelNumber = strtol(option->c_str(), &p, 10);
	if (!*p && modelNumber >= minNumber && modelNumber <= maxNumber) {
		int idx = modelNumber - pageSize * route.offset - 1;
		DialogRoute editModelRoute = DialogRoute(DialogRouteType::MODEL_EDIT, route.modelName, route.offset, route.search);
		editModelRoute.dialogForm = repositoryHub->getModelEditDialogForm(route, idx);
		pushRoute(editModelRoute);
	}
	else {
		*option = "q";
	}
}

DialogProcessor::DialogProcessor(RepositoryHub* repositoryHub, string initialText, bool showSearchAndPagination) {
	this->repositoryHub = repositoryHub;
	this->showSearchAndPagination = showSearchAndPagination;
	pushRoute(DialogRoute(DialogRouteType::ENTRY, initialText));
}


bool DialogProcessor::isFinished() {
	return routes.size() == 0;
}

void DialogProcessor::processRoute() {
	if (routes.size() == 0) {
		return;
	}

	system("cls");
	string option = "";
	DialogRoute route = routes[routes.size() - 1];

	if (!route.errorMessage.empty()) {
		cout << route.errorMessage << endl << endl;
		if (route.type != DialogRouteType::MODEL_PICK) {
			routes[routes.size() - 1].errorMessage = "";
		}
	}

	switch (route.type) {

	case DialogRouteType::ENTRY:
		processRouteEntry(route, &option);
		break;

	case DialogRouteType::MODEL_VIEW:
		processRouteModelView(route, &option);
		break;

	case DialogRouteType::MODEL_SEARCH:
		processRouteModelSearch(route, &option);
		break;

	case DialogRouteType::MODEL_DELETE:
		processRouteModelDelete(route, &option);
		break;

	case DialogRouteType::MODEL_ADD:
		processRouteModelAdd(route, &option);
		break;

	case DialogRouteType::MODEL_EDIT_SELECT:
		processRouteModelEditSelect(route, &option);
		break;

	case DialogRouteType::MODEL_EDIT:
		processRouteModelEdit(route, &option);
		break;

	case DialogRouteType::MODEL_PICK:
		processRouteModelPick(route, &option);
		break;
	}

	if (option == "q") {
		popRoute();
		return;
	}
}
