#ifndef REPOSITORY_HUB_H
#define REPOSITORY_HUB_H

#include "CityRepository.h"
#include "RouteRepository.h"
#include "JobRepository.h"
#include "ClientRepository.h"
#include "RepositoryNotFound.h"
#include "../dialog/DialogForm.h"

class RepositoryHub {

protected:
	DbConnector dbConnector;

public:
	CityRepository cityRepository;
	RouteRepository routeRepository;
	JobRepository jobRepository;
	ClientRepository clientRepository;
	RepositoryHub() {
		cityRepository = CityRepository();
		routeRepository = RouteRepository();
		jobRepository = JobRepository();
		clientRepository = ClientRepository();
	}

	RepositoryHub(DbConnector dbConnector) {
		cityRepository = CityRepository(dbConnector);
		routeRepository = RouteRepository(dbConnector, &cityRepository);
		jobRepository = JobRepository(dbConnector);
		clientRepository = ClientRepository(dbConnector);
	}

	void init() {
		cityRepository.checkTableExists();
		routeRepository.checkTableExists();
		jobRepository.checkTableExists();
		clientRepository.checkTableExists();
	}

	void getModelViewRouteData(DialogRoute route, int* totalModelsCount, int* pageSize, string search, vector<string>* modelLabels) {
		if (route.modelName == "Cities") {
			cityRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Routes") {
			routeRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Jobs") {
			jobRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Clients") {
			clientRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
	}

	int getModelId(DialogRoute route, int number, string search, int offset) {
		if (route.modelName == "Cities") {
			return cityRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Routes") {
			return routeRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Jobs") {
			return jobRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Clients") {
			return clientRepository.loadModels(search, offset)[number].id;
		}
	}

	DialogForm getModelAddDialogForm(DialogRoute route) {
		if (route.modelName == "Cities") {
			return DialogForm({ DialogFormStep("City name", DialogFormFieldType::FT_STRING, "city_name", "") });
		}
		else if (route.modelName == "Jobs") {
			return DialogForm({ DialogFormStep("Job title", DialogFormFieldType::FT_STRING, "job_title", "") });
		}
		else if (route.modelName == "Clients") {
			return DialogForm({
				DialogFormStep("Company name", DialogFormFieldType::FT_STRING, "company_name", ""),
				DialogFormStep("Postal address", DialogFormFieldType::FT_STRING, "postal_address", ""),
				DialogFormStep("Phone number", DialogFormFieldType::FT_STRING, "phone_number", ""),
				DialogFormStep("Fax number", DialogFormFieldType::FT_STRING, "fax_number", ""),
				DialogFormStep("Email", DialogFormFieldType::FT_STRING, "email", ""),
				});
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
			return DialogForm(model.id, { DialogFormStep("Job Title", DialogFormFieldType::FT_STRING, "job_title", model.jobTitle) });
		}
		else if (route.modelName == "Clients") {
			Client model = clientRepository.loadModels(route.search, route.offset * jobRepository.pageSize)[number];
			return DialogForm(model.id, { 
				DialogFormStep("Company Name", DialogFormFieldType::FT_STRING, "company_name", model.companyName),
				DialogFormStep("Postal Address", DialogFormFieldType::FT_STRING, "postal_address", model.postalAddress),
				DialogFormStep("Phone Number", DialogFormFieldType::FT_STRING, "phone_number", model.phoneNumber),
				DialogFormStep("Fax Number", DialogFormFieldType::FT_STRING, "fax_number", model.faxNumber),
				DialogFormStep("Email", DialogFormFieldType::FT_STRING, "email", model.email),
				});
		}
		else if (route.modelName == "Routes") {
			Route model = routeRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			return DialogForm(model.id, {
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
		}
		else if (route.modelName == "Routes") {
			Route routeM = Route(route.dialogForm.modelId);
			routeM.routeCost = route.dialogForm.steps[0].iValue;
			routeM.setDepartureCity(cityRepository.loadModelById(route.dialogForm.steps[1].iValue));
			routeM.setDestinationCity(cityRepository.loadModelById(route.dialogForm.steps[2].iValue));
			return routeRepository.saveModel(routeM);
		}
		else if (route.modelName == "Jobs") {
			return jobRepository.saveModel(Job(route.dialogForm.modelId, route.dialogForm.steps[0].sValue));
		}
		else if (route.modelName == "Clients") {
			return clientRepository.saveModel(Client(
				route.dialogForm.modelId, 
				route.dialogForm.steps[0].sValue, 
				route.dialogForm.steps[1].sValue, 
				route.dialogForm.steps[2].sValue, 
				route.dialogForm.steps[3].sValue, 
				route.dialogForm.steps[4].sValue
			));
		}

		return false;
	}

	bool deleteModelByModelNumber(DialogRoute route, int number) {
		if (route.modelName == "Cities") {
			City model = cityRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			cityRepository.deleteModel(model);
		}
		else if (route.modelName == "Routes") {
			Route model = routeRepository.loadModels(route.search, route.offset * routeRepository.pageSize)[number];
			routeRepository.deleteModel(model);
		}
		else if (route.modelName == "Jobs") {
			Job model = jobRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			jobRepository.deleteModel(model);
		}
		else if (route.modelName == "Clients") {
			Client model = clientRepository.loadModels(route.search, route.offset * cityRepository.pageSize)[number];
			clientRepository.deleteModel(model);
		}

		return true;
	}

};

#endif