#include "EmployeeRepository.h"
#include "../DbConnector.h"
#include "../models/Employee.h"

int EmployeeRepository::loadModelsCount() {
    return loadModelsCount("");
}

int EmployeeRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "WHERE firstname LIKE '%" + search + "%' OR patronymic LIKE '%" + search + "%' OR lastname LIKE '%" + search + "%' OR CAST(date_of_birth AS TEXT) LIKE '%" + search + "%' OR residential_address LIKE '%" + search + "%' OR t2.job_title LIKE '%" + search + "%' OR CAST(salary AS TEXT) LIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("employees", searchQuery, "LEFT JOIN jobs AS t2 on t1.job_id = t2.id");
}

vector<Employee> EmployeeRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<Employee> EmployeeRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<Employee> EmployeeRepository::loadModels() {
    return loadModels("", 0);
}

vector<Employee> EmployeeRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<Employee> newModels = {};

    if (!dbConnector.isConnected()) {
        //std::cout << "ASDSADsa " << models.size() << " " << cityRepository->loadModelsCount() << " " << cityRepository->loadModels().size() << std::endl;
        for (int i = 0; i < models.size(); i++) {
            Employee employee = Employee(models[i].id);
            employee.firstname = models[i].firstname;
            employee.patronymic = models[i].patronymic;
            employee.lastname = models[i].lastname;
            employee.dateOfBirth = models[i].dateOfBirth;
            employee.residentialAddress = models[i].residentialAddress;
            employee.setJobTitle(jobRepository->loadModelById(models[i].jobId));
            employee.salary = models[i].salary;
            newModels.push_back(employee); 
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
        searchString = "WHERE firstname LIKE '%" + search + "%' OR patronymic LIKE '%" + search + "%' OR lastname LIKE '%" + search + "%' OR CAST(date_of_birth AS TEXT) LIKE '%" + search + "%' OR residential_address LIKE '%" + search + "%' OR t2.job_title LIKE '%" + search + "%' OR CAST(salary AS TEXT) LIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 2048, "select t1.id, firstname, patronymic, lastname, date_of_birth, residential_address, job_id, salary from employees AS t1 LEFT JOIN jobs AS t2 on t1.job_id = t2.id %s ORDER BY t1.id ASC LIMIT %d OFFSET %d", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int LEN = 30;
    SQLCHAR firstname[LEN], patronymic[LEN], lastname[LEN], dateOfBirth[LEN], residentialAddress[50];
    SQLLEN employeeId = 0, jobId = 0, salary = 0, len = 0;
    
    retCode = SQLBindCol(hStmt, 1, SQL_C_LONG, &employeeId, 1, &len);
    retCode = SQLBindCol(hStmt, 2, SQL_C_CHAR, &firstname, LEN, &len);
    retCode = SQLBindCol(hStmt, 3, SQL_C_CHAR, &patronymic, LEN, &len);
    retCode = SQLBindCol(hStmt, 4, SQL_C_CHAR, &lastname, LEN, &len);
    retCode = SQLBindCol(hStmt, 5, SQL_C_CHAR, &dateOfBirth, LEN, &len);
    retCode = SQLBindCol(hStmt, 6, SQL_C_CHAR, &residentialAddress, 50, &len);
    retCode = SQLBindCol(hStmt, 7, SQL_C_LONG, &jobId, 1, &len);
    retCode = SQLBindCol(hStmt, 8, SQL_C_LONG, &salary, 1, &len);

    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            Employee employee = Employee(employeeId);
            employee.firstname = string((char*)firstname);
            employee.patronymic = string((char*)patronymic);
            employee.lastname = string((char*)lastname);
            employee.dateOfBirth = string((char*)dateOfBirth);
            employee.residentialAddress = string((char*)residentialAddress);
            employee.setJobTitle(jobRepository->loadModelById(jobId));
            employee.salary = salary;
            newModels.push_back(employee);
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
