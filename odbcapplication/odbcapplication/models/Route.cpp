#include "Route.h"

Route::Route() {
	this->id = 0;
	this->routeCost = 0;
	setDepartureCity(City());
	setDestinationCity(City());
}

Route::Route(int routeCost) {
	this->id = 0;
	this->routeCost = routeCost;
	setDepartureCity(City());
	setDestinationCity(City());
}

void Route::setDepartureCity(City departureCity) {
	this->departureCity = departureCity;
	this->departureCityId = departureCity.id;
}

void Route::setDestinationCity(City destinationCity) {
	this->destinationCity = destinationCity;
	this->destinationCityId = destinationCity.id;
}

int Route::insert(HDBC hDBC) {
	HSTMT hStmt;
	SQLLEN cbValue = SQL_NTS;
	SQLCHAR* sql = (SQLCHAR*)"INSERT INTO routes (route_cost, departure_city_id, destination_city_id) VALUES (?, ?, ?) RETURNING id;";

	try {
		checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
		checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
		checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&routeCost, 0, &cbValue), "Bind route cost");
		checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&departureCityId, 0, &cbValue), "Bind departure city id");
		checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&destinationCityId, 0, &cbValue), "Bind destination city id");
		checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
		if (SQLFetch(hStmt) == SQL_SUCCESS) {
			SQLGetData(hStmt, 1, SQL_C_ULONG, &id, 0, NULL);
		}

		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	catch (ActiveRecordException& exc) {
		return 0;
	}

	return id;
}

bool Route::update(HDBC hDBC) {
	HSTMT hStmt;
	SQLLEN cbValue = SQL_NTS;
	SQLCHAR* sql = (SQLCHAR*)"UPDATE routes SET route_cost = ?, departure_city_id = ?, destination_city_id = ? WHERE id = ?;";

	try {
		checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
		checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
		checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&routeCost, 0, &cbValue), "Bind parameter");
		checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&departureCityId, 0, &cbValue), "Bind parameter");
		checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&destinationCityId, 0, &cbValue), "Bind parameter");
		checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
		checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	catch (ActiveRecordException& exc) {
		return false;
	}

	return true;
}

bool Route::remove(HDBC hDBC) {
	HSTMT hStmt;
	SQLLEN cbValue = SQL_NTS;
	SQLCHAR* sql = (SQLCHAR*)"DELETE FROM routes WHERE id = ?;";

	try {
		checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
		checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
		checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
		checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	catch (ActiveRecordException& exc) {
		cout << "delete failed: " << exc.retCode << " " << exc.message << endl;
		cin.get();
		return false;
	}

	return true;
}
