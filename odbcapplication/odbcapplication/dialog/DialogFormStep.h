#ifndef DIALOG_FORM_STEP_H
#define DIALOG_FORM_STEP_H

#include <iostream>
#include <vector>
#include "DialogRoute.h"
#include "DialogFormFieldType.h"

using namespace std;

class DialogFormStep {

public:
	string title;
	string fieldName;
	string sValue = "";
	int iValue = 0;
	float fValue = 0.0;
	DialogFormFieldType type;

	DialogFormStep(string title, DialogFormFieldType type, string fieldName) {
		this->title = title;
		this->type = type;
		this->fieldName = fieldName;
	}

	DialogFormStep(string title, DialogFormFieldType type, string fieldName, string value) {
		this->title = title;
		this->type = type;
		this->fieldName = fieldName;
		setValue(value);
	}

	DialogFormStep(string title, DialogFormFieldType type, string fieldName, int value) {
		this->title = title;
		this->type = type;
		this->fieldName = fieldName;
		setValue(value);
	}

	DialogFormStep(string title, DialogFormFieldType type, string fieldName, float value) {
		this->title = title;
		this->type = type;
		this->fieldName = fieldName;
		setValue(value);
	}

	void setValue(string sValue) {
		this->sValue = sValue;
	}

	void setValue(int iValue) {
		this->iValue = iValue;
	}

	void setValue(float fValue) {
		this->fValue = fValue;
	}
};

#endif