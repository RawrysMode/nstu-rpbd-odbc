#ifndef DIALOG_PROCESSOR_H
#define DIALOG_PROCESSOR_H

#include <iostream>
#include <vector>
#include "DialogRoute.h"
#include "../repositories/RepositoryHub.h"

using namespace std;

class DialogProcessor {

private:
	vector<DialogRoute> routes = {};
	RepositoryHub* repositoryHub;
	bool showSearchAndPagination = true;

	void popRoute();
	void pushRoute(DialogRoute route);
	void displayModelLabels(vector<string> modelLabels, int pageSize, int offset, int totalModelsCount, string search);
	void processRouteEntry(DialogRoute route, string* option);
	void processRouteModelView(DialogRoute route, string* option);
	void processRouteModelSearch(DialogRoute route, string* option);
	void processRouteModelDelete(DialogRoute route, string* option);
	void processRouteModelAdd(DialogRoute route, string* option);
	void processRouteModelEdit(DialogRoute route, string* option);
	void processRouteModelEditSelect(DialogRoute route, string* option);
	void processRouteModelPick(DialogRoute route, string* option);

public:
	DialogProcessor(RepositoryHub* repositoryHub, string initialText, bool showSearchAndPagination);
	bool isFinished();
	void processRoute();
};

#endif