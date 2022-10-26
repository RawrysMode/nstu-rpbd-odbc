#include "RouteRepository.h"
#include "../DbConnector.h"
#include "../models/Route.h"

int RouteRepository::loadModelsCount() {
	return loadModelsCount("");
}

int RouteRepository::loadModelsCount(string search) {
	std::string searchQuery = "";
	if (!search.empty()) {
		searchQuery = "WHERE t2.city_name LIKE '%" + search + "%' OR t3.city_name LIKE '%" + search + "%'";
	}

	if (!dbConnector.isConnected()) {
		return models.size();
	}

	return dbConnector.getRowsCount("routes", searchQuery, "LEFT JOIN cities AS t2 ON t1.departure_city_id = t2.id LEFT JOIN cities AS t3 ON t1.destination_city_id = t3.id");
}

vector<Route> RouteRepository::loadModels(int offset) {
	return loadModels("", offset);
}

vector<Route> RouteRepository::loadModels(string search) {
	return loadModels(search, 0);
}

vector<Route> RouteRepository::loadModels() {
	return loadModels("", 0);
}

vector<Route> RouteRepository::loadModels(string search, int offset) {
	RETCODE retCode;
	HSTMT hStmt;
	vector<Route> newModels = {};

	retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
	if (!dbConnector.checkRetCode(retCode)) {
		// std::cout << "Error allocating operator handle." << std::endl;
		return models;
	}

	SQLCHAR sql[1024];
	string searchString = "";
	if (!search.empty()) {
		searchString = "WHERE t2.city_name LIKE '%" + search + "%' OR t3.city_name LIKE '%" + search + "%'";
	}

	sprintf_s((char*)sql, 1024, "SELECT t1.id, route_cost, departure_city_id, destination_city_id FROM routes AS t1 LEFT JOIN cities AS t2 ON t1.departure_city_id = t2.id LEFT JOIN cities AS t3 ON t1.destination_city_id = t3.id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d;", searchString.c_str(), pageSize, offset);

	retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
	if (!dbConnector.checkRetCode(retCode)) {
		std::cout << "Error while making SQL query." << std::endl;
		return models;
	}

	SQLLEN routeId = 0, routeCost = 0, departureCityId = 0, destinationCityId = 0, len = 0, routeNameLen = 0;

	retCode = SQLBindCol(hStmt, 1, SQL_C_LONG, &routeId, 1, &len);
	retCode = SQLBindCol(hStmt, 2, SQL_C_LONG, &routeCost, 1, &len);
	retCode = SQLBindCol(hStmt, 3, SQL_C_LONG, &departureCityId, 1, &len);
	retCode = SQLBindCol(hStmt, 4, SQL_C_LONG, &destinationCityId, 1, &len);
	for (int i = 0; ; i++) {
		retCode = SQLFetch(hStmt);
		if (dbConnector.checkRetCode(retCode)) {
			Route route = Route(routeId);
			route.routeCost = routeCost;
			route.setDepartureCity(City((long long)departureCityId).load(dbConnector.getDBC()));
			route.setDestinationCity(City((long long)departureCityId).load(dbConnector.getDBC()));
			newModels.push_back(route);
		}
		else {
			break;
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return models = newModels;
}
