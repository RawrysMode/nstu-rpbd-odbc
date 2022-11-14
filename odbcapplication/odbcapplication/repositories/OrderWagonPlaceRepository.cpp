#include "OrderWagonPlaceRepository.h"
#include "../DbConnector.h"
#include "../models/Order.h"

int OrderWagonPlaceRepository::loadModelsCount() {
    return loadModelsCount("");
}

int OrderWagonPlaceRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "where o.wagon_number::TEXT LIKE '%" + search + "%' OR o.shipping_date::text ILIKE '%" + search + "%' OR o.shipping_cost::TEXT LIKE '%" + search + "%' OR c.company_name ILIKE '%" + search + "%' OR space_number::TEXT LIKE '%" + search + "%' OR size ILIKE '%" + search + "%' OR weight LIKE '%" + search + "%' OR insurance_cost LIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("order_wagon_places", searchQuery, "left join orders o on t1.order_id = o.id left join clients c on o.client_id = c.id");
}

vector<OrderWagonPlace> OrderWagonPlaceRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<OrderWagonPlace> OrderWagonPlaceRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<OrderWagonPlace> OrderWagonPlaceRepository::loadModels() {
    return loadModels("", 0);
}

vector<OrderWagonPlace> OrderWagonPlaceRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<OrderWagonPlace> newModels = {};

    if (!dbConnector.isConnected()) {
        for (int i = 0; i < models.size(); i++) {
            OrderWagonPlace orderWagonPlace = OrderWagonPlace(models[i].id);
            orderWagonPlace.setOrder(orderRepository->loadModelById(models[i].orderId));
            orderWagonPlace.spaceNumber = models[i].spaceNumber;
            orderWagonPlace.size = models[i].size;
            orderWagonPlace.weight = models[i].weight;
            orderWagonPlace.insuranceCost = models[i].insuranceCost;
            newModels.push_back(orderWagonPlace);
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
        searchString = "where o.wagon_number::TEXT LIKE '%" + search + "%' OR o.shipping_date::text ILIKE '%" + search + "%' OR o.shipping_cost::TEXT LIKE '%" + search + "%' OR c.company_name ILIKE '%" + search + "%' OR space_number::TEXT LIKE '%" + search + "%' OR size ILIKE '%" + search + "%' OR weight LIKE '%" + search + "%' OR insurance_cost LIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 6000, "select t1.id, order_id, space_number, size, weight, insurance_cost from order_wagon_places as t1 left join orders o on t1.order_id = o.id left join clients c on o.client_id = c.id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int LEN = 30;
    SQLCHAR size[LEN], weight[LEN];
    SQLLEN orderWagonPlaceId = 0, orderId = 0, spaceNumber = 0, insuranceCost = 0, len = 0;

    retCode = SQLBindCol(hStmt, 1, SQL_C_LONG, &orderWagonPlaceId, 1, &len);
    retCode = SQLBindCol(hStmt, 2, SQL_C_LONG, &orderId, 1, &len);
    retCode = SQLBindCol(hStmt, 3, SQL_C_LONG, &spaceNumber, 1, &len);
    retCode = SQLBindCol(hStmt, 4, SQL_C_CHAR, &size, LEN, &len);
    retCode = SQLBindCol(hStmt, 5, SQL_C_CHAR, &weight, LEN, &len);
    retCode = SQLBindCol(hStmt, 6, SQL_C_LONG, &insuranceCost, 1, &len);


    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            OrderWagonPlace orderWagonPlace = OrderWagonPlace(orderWagonPlaceId);
            orderWagonPlace.setOrder(orderRepository->loadModelById(orderId));
            orderWagonPlace.spaceNumber = spaceNumber;
            orderWagonPlace.size = string((char*)size);
            orderWagonPlace.weight = string((char*)weight);
            orderWagonPlace.insuranceCost = insuranceCost;
            newModels.push_back(orderWagonPlace);
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
