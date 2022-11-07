#include "CityRepository.h"
#include "../DbConnector.h"
#include "../models/City.h"

int CityRepository::loadModelsCount() {
    return loadModelsCount("");
}

int CityRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "WHERE city_name LIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("cities", searchQuery, "");
}

vector<City> CityRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<City> CityRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<City> CityRepository::loadModels() {
    return loadModels("", 0);
}

vector<City> CityRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<City> newModels = {};

    if (!dbConnector.isConnected()) {
        return models;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
    if (!dbConnector.checkRetCode(retCode)) {
        return models;
    }

    SQLCHAR sql[256];
    string searchString = "";
    if (!search.empty()) {
        searchString = "WHERE city_name LIKE '%" + search + "%'";
    }

    sprintf_s((char *)sql, 256, "SELECT id, city_name FROM cities %s ORDER BY city_name ASC LIMIT %d OFFSET %d;", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int CITY_NAME_LEN = 25;
    SQLCHAR cityName[CITY_NAME_LEN];
    SQLLEN cityId = 0, cityIdLen = 0, cityNameLen = 0;

    retCode = SQLBindCol(hStmt, 1, SQL_C_USHORT, &cityId, 2, &cityIdLen);
    retCode = SQLBindCol(hStmt, 2, SQL_C_CHAR, &cityName, CITY_NAME_LEN, &cityNameLen);
    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            newModels.push_back(City((long long)cityId, string((char *)cityName)));
        } else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
