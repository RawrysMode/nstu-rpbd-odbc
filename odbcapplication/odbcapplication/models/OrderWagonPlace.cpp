#include "OrderWagonPlace.h"

OrderWagonPlace::OrderWagonPlace() {
    this->id = 0;
    this->setOrder(Order());
    this->spaceNumber;
    this->size;
    this->weight;
    this->insuranceCost;
}

OrderWagonPlace::OrderWagonPlace(int id) {
    this->id = id;
    this->setOrder(Order());
    this->spaceNumber;
    this->size;
    this->weight;
    this->insuranceCost;
}
/*
void OrderWagonPlace::setClient(Client client) {
    this->client = client;
    this->clientId = client.id;
}
*/

void OrderWagonPlace::setOrder(Order order) {
    this->order = order;
    this->orderId = order.id;
}

int OrderWagonPlace::insert(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"INSERT INTO order_wagon_places (order_id, space_number, size, weight, insurance_cost) VALUES (?, ?, ?, ?, ?) RETURNING id;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&orderId, 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&spaceNumber, 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(size.c_str()), 0, (SQLPOINTER)size.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(weight.c_str()), 0, (SQLPOINTER)weight.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&insuranceCost, 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
        if (SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 1, SQL_C_ULONG, &id, 0, NULL);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        cout << exc.message << endl;
        cin.get();

        return 0;
    }

    return id;
}

bool OrderWagonPlace::update(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"UPDATE order_wagon_places SET order_id = ?, space_number = ?, size = ?, weight = ?, insurance_cost = ? WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&orderId, 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&spaceNumber, 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(size.c_str()), 0, (SQLPOINTER)size.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(weight.c_str()), 0, (SQLPOINTER)weight.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&insuranceCost, 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        return false;
    }

    return true;
}

bool Order::remove(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"DELETE FROM order_wagon_place WHERE id = ?;";

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
