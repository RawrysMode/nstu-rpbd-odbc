#include "ClientRepository.h"
#include "../DbConnector.h"
#include "../models/Client.h"

int ClientRepository::loadModelsCount() {
    return loadModelsCount("");
}

int ClientRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "WHERE company_name LIKE '%" + search + "%' OR postal_address LIKE '%" + search + "%' OR phone_number LIKE '%" + search + "%' OR fax_number LIKE '%" + search + "%' OR email LIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("clients", searchQuery, "");
}

vector<Client> ClientRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<Client> ClientRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<Client> ClientRepository::loadModels() {
    return loadModels("", 0);
}

vector<Client> ClientRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<Client> newModels = {};

    if (!dbConnector.isConnected()) {
        return models;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
    if (!dbConnector.checkRetCode(retCode)) {
        return models;
    }

    SQLCHAR sql[1024];
    string searchString = "";
    if (!search.empty()) {
        searchString = "WHERE company_name LIKE '%" + search + "%' OR postal_address LIKE '%" + search + "%' OR phone_number LIKE '%" + search + "%' OR fax_number LIKE '%" + search + "%' OR email LIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 1024, "SELECT id, company_name, postal_address, phone_number, fax_number, email FROM clients %s ORDER BY company_name ASC LIMIT %d OFFSET %d;", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int LEN = 25;
    SQLCHAR companyName[LEN], postalAddress[LEN], phoneNumber[LEN], faxNumber[LEN], email[LEN];
    SQLLEN clientId = 0, len = 0;

    retCode = SQLBindCol(hStmt, 1, SQL_C_USHORT, &clientId, 2, &len);
    retCode = SQLBindCol(hStmt, 2, SQL_C_CHAR, &companyName, LEN, &len);
    retCode = SQLBindCol(hStmt, 3, SQL_C_CHAR, &postalAddress, LEN, &len);
    retCode = SQLBindCol(hStmt, 4, SQL_C_CHAR, &phoneNumber, LEN, &len);
    retCode = SQLBindCol(hStmt, 5, SQL_C_CHAR, &faxNumber, LEN, &len);
    retCode = SQLBindCol(hStmt, 6, SQL_C_CHAR, &email, LEN, &len);
    
    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            newModels.push_back(Client((long long)clientId, string((char*)companyName), string((char*)postalAddress), string((char*)phoneNumber), string((char*)faxNumber), string((char*)email)));
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
