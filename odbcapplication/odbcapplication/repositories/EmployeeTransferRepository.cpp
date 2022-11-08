#include "EmployeeTransferRepository.h"
#include "../DbConnector.h"
#include "../models/EmployeeTransfer.h"

int EmployeeTransferRepository::loadModelsCount() {
    return loadModelsCount("");
}

int EmployeeTransferRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "where e.firstname ILIKE '%" + search + "%' OR e.patronymic ILIKE '%" + search + "%' OR e.lastname ILIKE '%" + search + "%' OR transfer_reason ILIKE '%" + search + "%' OR CAST(order_number as TEXT) ILIKE '%" + search + "%' OR CAST(order_date as TEXT) ILIKE '%" + search + "%' OR j.job_title ILIKE '%" + search + "%' OR j2.job_title ILIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("employee_transfers", searchQuery, "left join employees e on t1.employee_id = e.id left join jobs j on t1.old_job_id = j.id left join jobs j2 on t1.old_job_id = j2.id");
}

vector<EmployeeTransfer> EmployeeTransferRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<EmployeeTransfer> EmployeeTransferRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<EmployeeTransfer> EmployeeTransferRepository::loadModels() {
    return loadModels("", 0);
}

vector<EmployeeTransfer> EmployeeTransferRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<EmployeeTransfer> newModels = {};

    if (!dbConnector.isConnected()) {
        for (int i = 0; i < models.size(); i++) {
            EmployeeTransfer employeeTransfer = EmployeeTransfer(models[i].id);
            employeeTransfer.setEmployee(employeeRepository->loadModelById(models[i].employeeId));
            employeeTransfer.transferReason = models[i].transferReason;
            employeeTransfer.setOldJob(jobRepository->loadModelById(models[i].oldJobId));
            employeeTransfer.setNewJob(jobRepository->loadModelById(models[i].newJobId));
            employeeTransfer.orderNumber = models[i].orderNumber;
            employeeTransfer.orderDate = models[i].orderDate;

            newModels.push_back(employeeTransfer);
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
        searchString = "where e.firstname ILIKE '%" + search + "%' OR e.patronymic ILIKE '%" + search + "%' OR e.lastname ILIKE '%" + search + "%' OR transfer_reason ILIKE '%" + search + "%' OR CAST(order_number as TEXT) ILIKE '%" + search + "%' OR CAST(order_date as TEXT) ILIKE '%" + search + "%' OR j.job_title ILIKE '%" + search + "%' OR j2.job_title ILIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 6000, "select t1.id, employee_id, transfer_reason, old_job_id, new_job_id, order_number, order_date from employee_transfers as t1 left join employees e on t1.employee_id = e.id left join jobs j on t1.old_job_id = j.id left join jobs j2 on t1.old_job_id = j2.id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int LEN = 30;
    SQLCHAR transferReason[LEN], orderDate[LEN];
    SQLLEN employeeTransferId = 0, employeeId = 0, oldJobId = 0, newJobId = 0, orderNumber = 0, len = 0;
    
    retCode = SQLBindCol(hStmt, 1, SQL_C_LONG, &employeeTransferId, 1, &len);
    retCode = SQLBindCol(hStmt, 2, SQL_C_LONG, &employeeId, 1, &len);
    retCode = SQLBindCol(hStmt, 3, SQL_C_CHAR, &transferReason, LEN, &len);
    retCode = SQLBindCol(hStmt, 4, SQL_C_LONG, &oldJobId, 1, &len);
    retCode = SQLBindCol(hStmt, 5, SQL_C_LONG, &newJobId, 1, &len);
    retCode = SQLBindCol(hStmt, 6, SQL_C_LONG, &orderNumber, 1, &len);
    retCode = SQLBindCol(hStmt, 7, SQL_C_CHAR, &orderDate, LEN, &len);

    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            EmployeeTransfer employeeTransfer = EmployeeTransfer(employeeTransferId);
            employeeTransfer.setEmployee(employeeRepository->loadModelById(employeeId));
            employeeTransfer.transferReason = string((char*)transferReason);
            employeeTransfer.setOldJob(jobRepository->loadModelById(oldJobId));
            employeeTransfer.setNewJob(jobRepository->loadModelById(newJobId));
            employeeTransfer.orderNumber = orderNumber;
            employeeTransfer.orderDate = string((char*)orderDate);

            newModels.push_back(employeeTransfer);
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
