#include "BankDetailsRepository.h"
#include "../DbConnector.h"
#include "../models/BankDetails.h"

int BankDetailsRepository::loadModelsCount() {
    return loadModelsCount("");
}

int BankDetailsRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "WHERE t2.company_name LIKE '%" + search + "%' OR bank_name LIKE '%" + search + "%' OR t3.city_name LIKE '%" + search + "%' OR tin LIKE '%" + search + "%' OR bank_account LIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("bank_details", searchQuery, "LEFT JOIN clients AS t2 on t1.client_id = t2.id LEFT JOIN cities AS t2 on t1.city_id = t2.id");
}

vector<BankDetails> BankDetailsRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<BankDetails> BankDetailsRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<BankDetails> BankDetailsRepository::loadModels() {
    return loadModels("", 0);
}

vector<BankDetails> BankDetailsRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<BankDetails> newModels = {};

    if (!dbConnector.isConnected()) {
        for (int i = 0; i < models.size(); i++) {
            BankDetails bankDetails = BankDetails(models[i].id);
            bankDetails.setCompanyName(clientRepository->loadModelById(models[i].companyId));
            bankDetails.bankAccount = models[i].bankAccount;
            bankDetails.setCityName(cityRepository->loadModelById(models[i].cityId));
            bankDetails.taxpayerIN = models[i].taxpayerIN;
            bankDetails.bankAccount = models[i].bankAccount;

            newModels.push_back(bankDetails);
        }

        return newModels;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
    if (!dbConnector.checkRetCode(retCode)) {
        return models;
    }

    SQLCHAR sql[2048];
    string searchString = "";
    if (!search.empty()) {
        searchString = "WHERE t2.company_name LIKE '%" + search + "%' OR bank_name LIKE '%" + search + "%' OR t3.city_name LIKE '%" + search + "%' OR tin LIKE '%" + search + "%' OR bank_account LIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 2048, "select t1.id, client_id, bank_name, city_id, tin, bank_account from bank_details AS t1 LEFT JOIN clients AS t2 on t1.client_id = t2.id LEFT JOIN cities AS t2 on t1.city_id = t2.id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int LEN = 30;
    SQLCHAR bankName[LEN], taxpayerIN[LEN], bankAccount[LEN];
    SQLLEN bankDetailsId = 0, clientId = 0, cityId = 0, len = 0;

    retCode = SQLBindCol(hStmt, 1, SQL_C_LONG, &bankDetailsId, 1, &len);
    retCode = SQLBindCol(hStmt, 2, SQL_C_LONG, &clientId, 1, &len);
    retCode = SQLBindCol(hStmt, 3, SQL_C_CHAR, &bankName, LEN, &len);
    retCode = SQLBindCol(hStmt, 4, SQL_C_LONG, &cityId, 1, &len);
    retCode = SQLBindCol(hStmt, 5, SQL_C_CHAR, &taxpayerIN, LEN, &len);
    retCode = SQLBindCol(hStmt, 6, SQL_C_CHAR, &bankAccount, LEN, &len);

    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            BankDetails bankDetails = BankDetails(bankDetailsId);
            bankDetails.setCompanyName(clientRepository->loadModelById(clientId));
            bankDetails.bankAccount = string((char*)bankAccount);
            bankDetails.setCityName(cityRepository->loadModelById(cityId));
            bankDetails.taxpayerIN = string((char*)taxpayerIN);
            bankDetails.bankAccount = string((char*)bankAccount);

            newModels.push_back(bankDetails);
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
