#ifndef REPOSITORY_HUB_H
#define REPOSITORY_HUB_H

#include "CityRepository.h"
#include "RouteRepository.h"
#include "JobRepository.h"
#include "ClientRepository.h"
#include "EmployeeRepository.h"
#include "BankDetailsRepository.h"
#include "EmployeeTransferRepository.h"
#include "OrderRepository.h"
#include "OrderWagonPlaceRepository.h"
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
	EmployeeRepository employeeRepository;
	BankDetailsRepository bankDetailsRepository;
	EmployeeTransferRepository employeeTransferRepository;
	OrderRepository orderRepository;
	OrderWagonPlaceRepository orderWagonPlaceRepository;

	RepositoryHub() {
		cityRepository = CityRepository();
		routeRepository = RouteRepository();
		jobRepository = JobRepository();
		clientRepository = ClientRepository();
		employeeRepository = EmployeeRepository();
		bankDetailsRepository = BankDetailsRepository();
		employeeTransferRepository = EmployeeTransferRepository();
		orderRepository = OrderRepository();
		orderWagonPlaceRepository = OrderWagonPlaceRepository();
	}

	RepositoryHub(DbConnector dbConnector) {
		cityRepository = CityRepository(dbConnector);
		routeRepository = RouteRepository(dbConnector, &cityRepository);
		jobRepository = JobRepository(dbConnector);
		clientRepository = ClientRepository(dbConnector);
		employeeRepository = EmployeeRepository(dbConnector, &jobRepository);
		bankDetailsRepository = BankDetailsRepository(dbConnector, &clientRepository, &cityRepository);
		employeeTransferRepository = EmployeeTransferRepository(dbConnector, &employeeRepository, &jobRepository);
		orderRepository = OrderRepository(dbConnector, &clientRepository, &employeeRepository, &routeRepository);
		orderWagonPlaceRepository = OrderWagonPlaceRepository(dbConnector, &orderRepository);
	}

	void init() {
		cityRepository.checkTableExists();
		routeRepository.checkTableExists();
		jobRepository.checkTableExists();
		clientRepository.checkTableExists();
		employeeRepository.checkTableExists();
		bankDetailsRepository.checkTableExists();
		employeeTransferRepository.checkTableExists();
		orderRepository.checkTableExists();
		orderWagonPlaceRepository.checkTableExists();
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
		else if (route.modelName == "Employees") {
			employeeRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Bank Details") {
			bankDetailsRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Employee Transfers") {
			employeeTransferRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Orders") {
			orderRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
		}
		else if (route.modelName == "Order Wagon Places") {
			orderWagonPlaceRepository.getModelViewRouteData(route, totalModelsCount, pageSize, search, modelLabels);
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
		else if (route.modelName == "Employees") {
			return employeeRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Bank Details") {
			return bankDetailsRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Employee Transfers") {
			return employeeTransferRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Orders") {
			return orderRepository.loadModels(search, offset)[number].id;
		}
		else if (route.modelName == "Order Wagon Places") {
			return orderWagonPlaceRepository.loadModels(search, offset)[number].id;
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
				DialogFormStep("Email", DialogFormFieldType::FT_STRING, "email", "")
				});
		}
		else if (route.modelName == "Routes") {
			return DialogForm({
				DialogFormStep("Select departure city from the list: ", DialogFormFieldType::FT_CITY, "departure_city_id", ""),
				DialogFormStep("Select destination city from the list: ", DialogFormFieldType::FT_CITY, "destination_city_id", ""),
				DialogFormStep("Route cost", DialogFormFieldType::FT_INT, "route_cost", "")
				});
		}
		else if (route.modelName == "Employees") {
			return DialogForm({
				DialogFormStep("Firstname", DialogFormFieldType::FT_STRING, "firstname", ""),
				DialogFormStep("Patronymic", DialogFormFieldType::FT_STRING, "patronymic", ""),
				DialogFormStep("Lastname", DialogFormFieldType::FT_STRING, "lastname", ""),
				DialogFormStep("Date Of Birth", DialogFormFieldType::FT_STRING, "date_of_birth", ""),
				DialogFormStep("Residential Address", DialogFormFieldType::FT_STRING, "residential_address", ""),
				DialogFormStep("Select job from the list: ", DialogFormFieldType::FT_JOB, "job_id", ""),
				DialogFormStep("Salary", DialogFormFieldType::FT_INT, "salary", "")
				});
		}
		else if (route.modelName == "Bank Details") {
			return DialogForm({
				DialogFormStep("Select client from the list: ", DialogFormFieldType::FT_CLIENT, "client_id", ""),
				DialogFormStep("Bank Name", DialogFormFieldType::FT_STRING, "bank_name", ""),
				DialogFormStep("Select city from the list: ", DialogFormFieldType::FT_CITY, "city_id", ""),
				DialogFormStep("Taxpayer Identification Number", DialogFormFieldType::FT_STRING, "tin", ""),
				DialogFormStep("Bank Account", DialogFormFieldType::FT_STRING, "bank_account", "")
				});
		}
		else if (route.modelName == "Employee Transfers") {
			return DialogForm({
				DialogFormStep("Select employee from the list: ", DialogFormFieldType::FT_EMPLOYEE, "employee_id", ""),
				DialogFormStep("Transfer Reason", DialogFormFieldType::FT_STRING, "transfer_reason", ""),
				DialogFormStep("Select old job title from the list: ", DialogFormFieldType::FT_JOB, "old_job_id", ""),
				DialogFormStep("Select new job title from the list: ", DialogFormFieldType::FT_JOB, "new_job_id", ""),
				DialogFormStep("Order Number", DialogFormFieldType::FT_INT, "order_number", ""),
				DialogFormStep("Order Date", DialogFormFieldType::FT_STRING, "order_date", "")
				});
		}
		else if (route.modelName == "Orders") {
			return DialogForm({
				DialogFormStep("Select client from the list: ", DialogFormFieldType::FT_CLIENT, "client_id", ""),
				DialogFormStep("Select employee from the list: ", DialogFormFieldType::FT_EMPLOYEE, "employee_id", ""),
				DialogFormStep("Order Date", DialogFormFieldType::FT_STRING, "order_date", ""),
				DialogFormStep("Select route from the list: ", DialogFormFieldType::FT_ROUTE, "route_id", ""),
				DialogFormStep("Wagon Number", DialogFormFieldType::FT_INT, "wagon_number", ""),
				DialogFormStep("Shipping Date", DialogFormFieldType::FT_STRING, "shipping_date", ""),
				DialogFormStep("Shipping cost", DialogFormFieldType::FT_INT, "shipping_cost", ""),
				DialogFormStep("Invoice Number", DialogFormFieldType::FT_STRING, "nvc", "")
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
			Client model = clientRepository.loadModels(route.search, route.offset * clientRepository.pageSize)[number];
			return DialogForm(model.id, {
				DialogFormStep("Company Name", DialogFormFieldType::FT_STRING, "company_name", model.companyName),
				DialogFormStep("Postal Address", DialogFormFieldType::FT_STRING, "postal_address", model.postalAddress),
				DialogFormStep("Phone Number", DialogFormFieldType::FT_STRING, "phone_number", model.phoneNumber),
				DialogFormStep("Fax Number", DialogFormFieldType::FT_STRING, "fax_number", model.faxNumber),
				DialogFormStep("Email", DialogFormFieldType::FT_STRING, "email", model.email)
				});
		}
		else if (route.modelName == "Routes") {
			Route model = routeRepository.loadModels(route.search, route.offset * routeRepository.pageSize)[number];
			return DialogForm(model.id, {
				DialogFormStep("Select departure city from the list: ", DialogFormFieldType::FT_CITY, "departure_city_id", model.departureCityId),
				DialogFormStep("Select destination city from the list: ", DialogFormFieldType::FT_CITY, "destination_city_id", model.destinationCityId),
				DialogFormStep("Route cost", DialogFormFieldType::FT_INT, "route_cost", model.routeCost)
				});
		}
		else if (route.modelName == "Employees") {
			Employee model = employeeRepository.loadModels(route.search, route.offset * employeeRepository.pageSize)[number];
			return DialogForm(model.id, {
				DialogFormStep("Firstname", DialogFormFieldType::FT_STRING, "firstname", model.firstname),
				DialogFormStep("Patronymic", DialogFormFieldType::FT_STRING, "patronymic", model.patronymic),
				DialogFormStep("Lastname", DialogFormFieldType::FT_STRING, "lastname", model.lastname),
				DialogFormStep("Date Of Birth", DialogFormFieldType::FT_STRING, "date_of_birth", model.dateOfBirth),
				DialogFormStep("Residential Address", DialogFormFieldType::FT_STRING, "residential_address", model.residentialAddress),
				DialogFormStep("Select job from the list", DialogFormFieldType::FT_JOB, "job_id", model.jobId),
				DialogFormStep("Salary", DialogFormFieldType::FT_INT, "salary", model.salary),
				});
		}
		else if (route.modelName == "Bank Details") {
			BankDetails model = bankDetailsRepository.loadModels(route.search, route.offset * bankDetailsRepository.pageSize)[number];
			return DialogForm(model.id, {
				DialogFormStep("Select client from the list: ", DialogFormFieldType::FT_CLIENT, "client_id", model.companyId),
				DialogFormStep("Bank Name", DialogFormFieldType::FT_STRING, "bank_name", model.bankName),
				DialogFormStep("Select city from the list: ", DialogFormFieldType::FT_CITY, "city_id", model.cityId),
				DialogFormStep("Taxpayer Identification Number", DialogFormFieldType::FT_STRING, "tin", model.taxpayerIN),
				DialogFormStep("Bank Account", DialogFormFieldType::FT_STRING, "bank_account", model.bankAccount)
				});
		}
		else if (route.modelName == "Employee Transfers") {
			EmployeeTransfer model = employeeTransferRepository.loadModels(route.search, route.offset * employeeTransferRepository.pageSize)[number];
			return DialogForm(model.id, {
				DialogFormStep("Select employee from the list: ", DialogFormFieldType::FT_EMPLOYEE, "employee_id", model.employeeId),
				DialogFormStep("Transfer Reason", DialogFormFieldType::FT_STRING, "transfer_reason", model.transferReason),
				DialogFormStep("Select old job title from the list: ", DialogFormFieldType::FT_JOB, "old_job_id", model.oldJobId),
				DialogFormStep("Select new job title from the list: ", DialogFormFieldType::FT_JOB, "new_job_id", model.newJobId),
				DialogFormStep("Order Number", DialogFormFieldType::FT_INT, "order_number", model.orderNumber),
				DialogFormStep("Order Date", DialogFormFieldType::FT_STRING, "order_date", model.orderDate)
				});
		}
		else if (route.modelName == "Orders") {
			Order model = orderRepository.loadModels(route.search, route.offset * employeeTransferRepository.pageSize)[number];
			return DialogForm(model.id, {
				DialogFormStep("Select client from the list: ", DialogFormFieldType::FT_CLIENT, "client_id", model.clientId),
				DialogFormStep("Select employee from the list: ", DialogFormFieldType::FT_EMPLOYEE, "employee_id", model.employeeId),
				DialogFormStep("Order Date", DialogFormFieldType::FT_STRING, "order_date", model.orderDate),
				DialogFormStep("Select route from the list: ", DialogFormFieldType::FT_ROUTE, "route_id", model.routeId),
				DialogFormStep("Wagon Number", DialogFormFieldType::FT_INT, "wagon_number", model.wagonNumber),
				DialogFormStep("Shipping Date", DialogFormFieldType::FT_STRING, "shipping_date", model.shippingDate),
				DialogFormStep("Shipping Cost", DialogFormFieldType::FT_INT, "shipping_cost", model.shippingCost),
				DialogFormStep("Invoice Number", DialogFormFieldType::FT_STRING, "nvc", model.invoiceNumber)
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
			routeM.setDepartureCity(cityRepository.loadModelById(route.dialogForm.steps[0].iValue));
			routeM.setDestinationCity(cityRepository.loadModelById(route.dialogForm.steps[1].iValue));
			routeM.routeCost = route.dialogForm.steps[2].iValue;
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
		else if (route.modelName == "Employees") {
			Employee employee = Employee(route.dialogForm.modelId);

			employee.firstname = route.dialogForm.steps[0].sValue;
			employee.patronymic = route.dialogForm.steps[1].sValue;
			employee.lastname = route.dialogForm.steps[2].sValue;
			employee.dateOfBirth = route.dialogForm.steps[3].sValue;
			employee.residentialAddress = route.dialogForm.steps[4].sValue;
			employee.setJobTitle(jobRepository.loadModelById(route.dialogForm.steps[5].iValue));
			employee.salary = route.dialogForm.steps[6].iValue;
			return employeeRepository.saveModel(employee);
		}
		else if (route.modelName == "Bank Details") {
			BankDetails bankDetails = BankDetails(route.dialogForm.modelId);
			bankDetails.setCompanyName(clientRepository.loadModelById(route.dialogForm.steps[0].iValue));
			bankDetails.bankName = route.dialogForm.steps[1].sValue;
			bankDetails.setCityName(cityRepository.loadModelById(route.dialogForm.steps[2].iValue));
			bankDetails.taxpayerIN = route.dialogForm.steps[3].sValue;
			bankDetails.bankAccount = route.dialogForm.steps[4].sValue;
			return bankDetailsRepository.saveModel(bankDetails);
		}
		else if (route.modelName == "Employee Transfers") {
			EmployeeTransfer employeeTransfer = EmployeeTransfer(route.dialogForm.modelId);
			employeeTransfer.setEmployee(employeeRepository.loadModelById(route.dialogForm.steps[0].iValue));
			employeeTransfer.transferReason = route.dialogForm.steps[1].sValue;
			employeeTransfer.setOldJob(jobRepository.loadModelById(route.dialogForm.steps[2].iValue));
			employeeTransfer.setNewJob(jobRepository.loadModelById(route.dialogForm.steps[3].iValue));
			employeeTransfer.orderNumber = route.dialogForm.steps[4].iValue;
			employeeTransfer.orderDate = route.dialogForm.steps[5].sValue;
			return employeeTransferRepository.saveModel(employeeTransfer);
		}
		else if (route.modelName == "Orders") {
			Order order = Order(route.dialogForm.modelId);
			order.setClient(clientRepository.loadModelById(route.dialogForm.steps[0].iValue));
			order.setEmployee(employeeRepository.loadModelById(route.dialogForm.steps[1].iValue));
			order.orderDate = route.dialogForm.steps[2].sValue;
			order.setRoute(routeRepository.loadModelById(route.dialogForm.steps[3].iValue));
			order.wagonNumber = route.dialogForm.steps[4].iValue;
			order.shippingDate = route.dialogForm.steps[5].sValue;
			order.shippingCost = route.dialogForm.steps[6].iValue;
			order.invoiceNumber = route.dialogForm.steps[7].sValue;
			return orderRepository.saveModel(order);
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
			Job model = jobRepository.loadModels(route.search, route.offset * jobRepository.pageSize)[number];
			jobRepository.deleteModel(model);
		}
		else if (route.modelName == "Clients") {
			Client model = clientRepository.loadModels(route.search, route.offset * clientRepository.pageSize)[number];
			clientRepository.deleteModel(model);
		}
		else if (route.modelName == "Employees") {
			Employee model = employeeRepository.loadModels(route.search, route.offset * employeeRepository.pageSize)[number];
			employeeRepository.deleteModel(model);
		}
		else if (route.modelName == "Bank Details") {
			BankDetails model = bankDetailsRepository.loadModels(route.search, route.offset * bankDetailsRepository.pageSize)[number];
			bankDetailsRepository.deleteModel(model);
		}
		else if (route.modelName == "Employee Transfers") {
			EmployeeTransfer model = employeeTransferRepository.loadModels(route.search, route.offset * employeeTransferRepository.pageSize)[number];
			employeeTransferRepository.deleteModel(model);
		}
		else if (route.modelName == "Orders") {
			Order model = orderRepository.loadModels(route.search, route.offset * orderRepository.pageSize)[number];
			orderRepository.deleteModel(model);
		}

		return true;
	}
};

#endif