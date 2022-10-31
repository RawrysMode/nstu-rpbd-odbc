#include "EmployeeRepository.h"
#include "../DbConnector.h"
#include "../models/Employee.h"

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

    if (!dbConnector.isConnected()) {
        //std::cout << "ASDSADsa " << models.size() << " " << cityRepository->loadModelsCount() << " " << cityRepository->loadModels().size() << std::endl;
        for (int i = 0; i < models.size(); i++) {
            Route route = Route(models[i].id);
            route.routeCost = models[i].routeCost;
            route.setDepartureCity(cityRepository->loadModelById(models[i].departureCityId));
            route.setDestinationCity(cityRepository->loadModelById(models[i].destinationCityId));
            newModels.push_back(route);
        }

        return newModels;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
    if (!dbConnector.checkRetCode(retCode)) {
        return models;
    }

    SQLCHAR sql[1024];
    string searchString = "";
    if (!search.empty()) {
        searchString = "WHERE t2.city_name LIKE '%" + search + "%' OR t3.city_name LIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 1024, "select t1.id, firstname, patronymic, lastname, date_of_birth, residential_address, job_id, salary from employees AS t1 LEFT JOIN jobs AS t2 on t1.job_id = t2.id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d", searchString.c_str(), pageSize, offset);

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
            route.setDepartureCity(cityRepository->loadModelById(departureCityId));
            route.setDestinationCity(cityRepository->loadModelById(destinationCityId));
            newModels.push_back(route);
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
