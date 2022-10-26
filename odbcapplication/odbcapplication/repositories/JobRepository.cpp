#include "JobRepository.h"
#include "../DbConnector.h"
#include "../models/Job.h"

int JobRepository::loadModelsCount() {
    return loadModelsCount("");
}

int JobRepository::loadModelsCount(string search) {
    std::string searchQuery = "";
    if (!search.empty()) {
        searchQuery = "WHERE job_title LIKE '%" + search + "%'";
    }

    if (!dbConnector.isConnected()) {
        return models.size();
    }

    return dbConnector.getRowsCount("jobs", searchQuery, "");
}

vector<Job> JobRepository::loadModels(int offset) {
    return loadModels("", offset);
}

vector<Job> JobRepository::loadModels(string search) {
    return loadModels(search, 0);
}

vector<Job> JobRepository::loadModels() {
    return loadModels("", 0);
}

vector<Job> JobRepository::loadModels(string search, int offset) {
    RETCODE retCode;
    HSTMT hStmt;
    vector<Job> newModels = {};

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getDBC(), &hStmt);
    if (!dbConnector.checkRetCode(retCode)) {
        // std::cout << "Error allocating operator handle." << std::endl;
        return models;
    }

    SQLCHAR sql[256];
    string searchString = "";
    if (!search.empty()) {
        searchString = "WHERE job_title LIKE '%" + search + "%'";
    }

    sprintf_s((char*)sql, 256, "SELECT id, job_title FROM jobs %s ORDER BY job_title ASC LIMIT %d OFFSET %d;", searchString.c_str(), pageSize, offset);

    retCode = SQLExecDirectA(hStmt, sql, SQL_NTS);
    if (!dbConnector.checkRetCode(retCode)) {
        std::cout << "Error while making SQL query." << std::endl;
        return models;
    }

    const int JOB_TITLE_LEN = 25;
    SQLCHAR jobTitle[JOB_TITLE_LEN];
    SQLLEN jobId = 0, jobIdLen = 0, jobNameLen = 0;

    retCode = SQLBindCol(hStmt, 1, SQL_C_USHORT, &jobId, 2, &jobIdLen);
    retCode = SQLBindCol(hStmt, 2, SQL_C_CHAR, &jobTitle, JOB_TITLE_LEN, &jobNameLen);
    for (int i = 0; ; i++) {
        retCode = SQLFetch(hStmt);
        if (dbConnector.checkRetCode(retCode)) {
            newModels.push_back(Job((long long)jobId, string((char*)jobTitle)));
        }
        else {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return models = newModels;
}
