#include "Order.h"

Order::Order() {
    this->id = 0;
    this->setClient(Client());
    this->setEmployee(Employee());
    this->orderDate;
    this->setRoute(Route());
    this->wagonNumber;
    this->shippingDate;
    this->shippingCost;
    this->invoiceNumber;
}

Order::Order(int id) {
    this->id = id;
    this->setClient(Client());
    this->setEmployee(Employee());
    this->orderDate;
    this->setRoute(Route());
    this->wagonNumber;
    this->shippingDate;
    this->shippingCost;
    this->invoiceNumber;
}

void Order::setClient(Client client) {
    this->client = client;
    this->clientId = client.id;
}

void Order::setEmployee(Employee employee) {
    this->employee = employee;
    this->employeeId = employee.id;
}

void Order::setRoute(Route route) {
    this->route = route;
    this->routeId = route.id;
}

int Order::insert(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"INSERT INTO orders (client_id, employee_id, order_date, route_id, wagon_number, shipping_date, shipping_cost, nvc) VALUES (?, ?, ?, ?, ?, ?, ?, ?) RETURNING id;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&clientId, 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&employeeId, 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(orderDate.c_str()), 0, (SQLPOINTER)orderDate.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&routeId, 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&wagonNumber, 0, &cbValue), "Bind 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(shippingDate.c_str()), 0, (SQLPOINTER)shippingDate.c_str(), 0, &cbValue), "Bind 6");
        checkRetCodeException(SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&shippingCost, 0, &cbValue), "Bind 7");
        checkRetCodeException(SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(invoiceNumber.c_str()), 0, (SQLPOINTER)invoiceNumber.c_str(), 0, &cbValue), "Bind 8");
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

bool Order::update(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"UPDATE employee_transfers SET client_id = ?, employee_id = ?, order_date = ?, route_id = ?, wagon_number = ?, shipping_date = ?, shipping_cost = ?, nvc = ? WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&clientId, 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&employeeId, 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(orderDate.c_str()), 0, (SQLPOINTER)orderDate.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&routeId, 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&wagonNumber, 0, &cbValue), "Bind 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(shippingDate.c_str()), 0, (SQLPOINTER)shippingDate.c_str(), 0, &cbValue), "Bind 6");
        checkRetCodeException(SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&shippingCost, 0, &cbValue), "Bind 7");
        checkRetCodeException(SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(invoiceNumber.c_str()), 0, (SQLPOINTER)invoiceNumber.c_str(), 0, &cbValue), "Bind 8");
        checkRetCodeException(SQLBindParameter(hStmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
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
    SQLCHAR* sql = (SQLCHAR*)"DELETE FROM orders WHERE id = ?;";

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
<<<<<<< Updated upstream
=======

Order Order::load(HDBC hDBC) {
    RETCODE retCode;
    SQLLEN cbValue = SQL_NTS;
    HSTMT hStmt;
    const int LEN = 30;
    SQLCHAR companyName[LEN], shippingDate[LEN];
    SQLLEN wagonNumber = 0, shippingCost = 0, orderId = 0, len = 0;

    SQLCHAR* sql = (SQLCHAR*)"select t1.id, c.company_name, wagon_number, shipping_date, shipping_cost from orders as t1 left join clients c on t1.client_id = c.id where t1.id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");

        checkRetCodeException(SQLBindCol(hStmt, 1, SQL_C_LONG, &orderId, 1, &len));
        checkRetCodeException(SQLBindCol(hStmt, 2, SQL_C_CHAR, &companyName, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 3, SQL_C_LONG, &wagonNumber, 1, &len));
        checkRetCodeException(SQLBindCol(hStmt, 4, SQL_C_CHAR, &shippingDate, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 5, SQL_C_LONG, &shippingCost, 1, &len));

        checkRetCodeException(SQLFetch(hStmt));
        return Order((long long)orderId, string((char*)companyName), (long long)wagonNumber, string((char*)shippingDate), (long long)shippingCost);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        cout << "Load employees failed: " << exc.retCode << " " << exc.message << endl;
    }

    return Order();
}
>>>>>>> Stashed changes
