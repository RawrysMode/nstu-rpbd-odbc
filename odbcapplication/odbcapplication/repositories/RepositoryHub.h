#ifndef REPOSITORY_HUB_H
#define REPOSITORY_HUB_H

#include "CityRepository.h"
#include "RouteRepository.h"
#include "JobRepository.h"
#include "RepositoryNotFound.h"
#include "../dialog/DialogForm.h"

class RepositoryHub {

protected: 
	DbConnector dbConnector;

public:
	CityRepository cityRepository;
	RouteRepository routeRepository;
	JobRepository jobRepository;
	RepositoryHub() {}

	RepositoryHub(DbConnector dbConnector) {
		cityRepository = CityRepository(dbConnector);
		routeRepository = RouteRepository(dbConnector);
		jobRepository = JobRepository(dbConnector);
	}

	void init() {
		cityRepository.checkTableExists();
		routeRepository.checkTableExists();
		jobRepository.checkTableExists();
	}

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* pageSize, string search, vector<string>* modelLabels) {
		if (route.modelName == "Cities") {
			cityRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		} else if (route.modelName == "Routes") {
			routeRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		} else if (route.modelName == "Jobs") {
			routeRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
	}

	int getModelId(DialogRoute route, int number, string search, int offset) {
		if (route.modelName == "Cities") {
			return cityRepository.loadModels(search, offset)[number].id;
		} else if (route.modelName == "Routes") {
			return routeRepository.loadModels(search, offset)[number].id;
		} else if (route.modelName == "Jobs") {
			return routeRepository.loadModels(search, offset)[number].id;
		}
	}

	DialogForm getModelAddDialogForm(DialogRoute route) {
		if (route.modelName == "Cities") {
			return DialogForm({ DialogFormStep("City name", DialogFormFieldType::FT_STRING, "city_name", "") });
		}
		else if (route.modelName == "Jobs") {
			return DialogForm({ DialogFormStep("Job title", DialogFormFieldType::FT_STRING, "job_title", "") });
		}
		else if (route.modelName == "Routes") {
			return DialogForm({ 
				DialogFormStep("Route cost", DialogFormFieldType::FT_INT, "route_cost", ""),
				DialogFormStep("Select departure city from the list: ", DialogFormFieldType::FT_CITY, "departure_city_id", ""),
				DialogFormStep("Select destination city from the list: ", DialogFormFieldType::FT_CITY, "destination_city_id", "")
			});
		}

		return DialogForm();
	}

	DialogForm getModelEditDialogForm(DialogRoute route, int number) {
		if (route.modelName == "Cities") {
			City model = cityRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			return DialogForm(model.id, { DialogFormStep("City name", DialogFormFieldType::FT_STRING, "city_name", model.name) });
		}
		else if (route.modelName == "Jobs") {
			Job model = jobRepository.loadModels(route.search, route.offset * jobRepository.pageSize)[number];
			return DialogForm(model.id, { DialogFormStep("Job Title", DialogFormFieldType::FT_STRING, "job title", model.jobTitle) });
		}
		else if (route.modelName == "Routes") {
			Route model = routeRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			return DialogForm({
				DialogFormStep("Route cost", DialogFormFieldType::FT_INT, "route_cost", model.routeCost),
				DialogFormStep("Select departure city from the list: ", DialogFormFieldType::FT_CITY, "departure_city_id", model.departureCityId),
				DialogFormStep("Select destination city from the list: ", DialogFormFieldType::FT_CITY, "destination_city_id", model.destinationCityId)
			});
		}

		return DialogForm();
	}

	bool saveModel(DialogRoute route) {
		if (route.modelName == "Cities") {
			return cityRepository.saveModel(City(route.dialogForm.modelId, route.dialogForm.steps[0].sValue));
		} else if (route.modelName == "Routes") {
			Route routeM = Route(route.dialogForm.modelId);
			routeM.routeCost = route.dialogForm.steps[0].iValue;
			routeM.setDepartureCity(City(route.dialogForm.steps[1].iValue));
			routeM.setDestinationCity(City(route.dialogForm.steps[2].iValue));
			return routeRepository.saveModel(routeM);
		}

		return false;
	}

	bool deleteModelByModelNumber(DialogRoute route, int number) {
		if (route.modelName == "Cities") {
			City model = cityRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			cityRepository.deleteModel(model);
		} else if (route.modelName == "Routes") {
			Route model = routeRepository.loadModels(route.search, route.offset * routeRepository.pageSize)[number];
			routeRepository.deleteModel(model);
		}

		return true;
	}

};

#endif