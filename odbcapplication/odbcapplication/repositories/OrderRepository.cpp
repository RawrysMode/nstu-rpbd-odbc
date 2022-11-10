#include "OrderRepository.h"
#include "../DbConnector.h"
#include "../models/Order.h"

int OrderRepository::loadModelsCount() {
    return loadModelsCount("");
}

int OrderRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "where c.company_name ILIKE '%" + search + "%' OR e.firstname ILIKE '%" + search + "%' OR e.patronymic ILIKE '%" + search + "%' OR e.lastname ILIKE '%" + search + "%' OR order_date::text ILIKE '%" + search + "%' OR c2.city_name ILIKE '%" + search + "%' OR r.route_cost::text LIKE '%" + search + "%' OR wagon_number::text LIKE '%" + search + "%' OR shipping_date::text ILIKE '%" + search + "%' OR shipping_cost::text LIKE '%" + search + "%' OR nvc ILIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("orders", searchQuery, "left join clients c on t1.client_id = c.id left join employees e on t1.employee_id = e.id left join routes r on t1.route_id = r.id left join cities c2 on c2.id = r.departure_city_id and c2.id = r.destination_city_id");
}

vector<Order> OrderRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<Order> OrderRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<Order> OrderRepository::loadModels() {
    return loadModels("", 0);
}

vector<Order> OrderRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<Order> newModels = {};

    if (!dbConnector.isConnected()) {
        for (int i = 0; i < models.size(); i++) {
            Order order = Order(models[i].id);
            order.setClient(clientRepository->loadModelById(models[i].clientId));
            order.setEmployee(employeeRepository->loadModelById(models[i].employeeId));
            order.orderDate = models[i].orderDate;
            order.setRoute(routeRepository->loadModelById(models[i].routeId));
            order.wagonNumber = models[i].wagonNumber;
            order.shippingDate = models[i].shippingDate;
            order.shippingCost = models[i].shippingCost;
            order.invoiceNumber = models[i].invoiceNumber;
            newModels.push_back(order);
        }

        return newModels;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
    if (!dbConnector.checkRetCode(retCode)) {
        return models;
    }

    SQLCHAR sql[6000];
    string searchString = "";
    if (!search.empty()) {
        searchString = "where c.company_name ILIKE '%" + search + "%' OR e.firstname ILIKE '%" + search + "%' OR e.patronymic ILIKE '%" + search + "%' OR e.lastname ILIKE '%" + search + "%' OR order_date::text ILIKE '%" + search + "%' OR c2.city_name ILIKE '%" + search + "%' OR r.route_cost::text LIKE '%" + search + "%' OR wagon_number::text LIKE '%" + search + "%' OR shipping_date::text ILIKE '%" + search + "%' OR shipping_cost::text LIKE '%" + search + "%' OR nvc ILIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 6000, "select t1.id, client_id, employee_id, order_date, route_id, wagon_number, shipping_date, shipping_cost, nvc from orders as t1 left join clients c on t1.client_id = c.id left join employees e on t1.employee_id = e.id left join routes r on t1.route_id = r.id left join cities c2 on c2.id = r.departure_city_id and c2.id = r.destination_city_id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int LEN = 30;
    SQLCHAR orderDate[LEN], shippingDate[LEN], invoiceNumber[LEN];
    SQLLEN orderId = 0, clientId = 0, employeeId = 0, routeId = 0, wagonNumber = 0, shippingCost = 0, len = 0;

    retCode = SQLBindCol(hStmt, 1, SQL_C_LONG, &orderId, 1, &len);
    retCode = SQLBindCol(hStmt, 2, SQL_C_LONG, &clientId, 1, &len);
    retCode = SQLBindCol(hStmt, 3, SQL_C_LONG, &employeeId, 1, &len);
    retCode = SQLBindCol(hStmt, 4, SQL_C_CHAR, &orderDate, LEN, &len);
    retCode = SQLBindCol(hStmt, 5, SQL_C_LONG, &routeId, 1, &len);
    retCode = SQLBindCol(hStmt, 6, SQL_C_LONG, &wagonNumber, 1, &len);
    retCode = SQLBindCol(hStmt, 7, SQL_C_CHAR, &shippingDate, LEN, &len);
    retCode = SQLBindCol(hStmt, 8, SQL_C_LONG, &shippingCost, 1, &len);
    retCode = SQLBindCol(hStmt, 9, SQL_C_CHAR, &invoiceNumber, LEN, &len);
    

    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            Order order = Order(orderId);
            order.setClient(clientRepository->loadModelById(clientId));
            order.setEmployee(employeeRepository->loadModelById(employeeId));
            order.orderDate = string((char*)orderDate);
            order.setRoute(routeRepository->loadModelById(routeId));
            order.wagonNumber = wagonNumber;
            order.shippingDate = string((char*)shippingDate);
            order.shippingCost = shippingCost;
            order.invoiceNumber = string((char*)invoiceNumber);
            newModels.push_back(order);
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
