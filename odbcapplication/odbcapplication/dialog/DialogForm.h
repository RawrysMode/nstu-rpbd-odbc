#ifndef DIALOG_FORM_H
#define DIALOG_FORM_H

#include <iostream>
#include <vector>
#include "DialogRoute.h"
#include "DialogFormStep.h"

using namespace std;

class DialogForm {
	

public:
	int currentStepId = 0;
	int modelId = 0;

	vector<DialogFormStep> steps = {};
	DialogForm() {}
	DialogForm(vector<DialogFormStep> steps) {
		this->steps = steps;
	}


	DialogForm(int modelId, vector<DialogFormStep> steps) {
		this->modelId = modelId;
		this->steps = steps;
	}

	bool isFinished() {
		return currentStepId >= steps.size();
	}

	void nextStep() {
		currentStepId++;
	}
};

#endif