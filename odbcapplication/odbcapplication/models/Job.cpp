#include "Job.h"

Job::Job() {
    this->id = 0;
    this->jobTitle = "";
}

Job::Job(long long id, string jobTitle) {
    this->id = id;
    this->jobTitle = jobTitle;
}

Job::Job(long long id) {
    this->id = id;
    this->jobTitle = "";
}

Job::Job(string jobTitle) {
    this->id = 0;
    this->jobTitle = jobTitle;
}

int Job::insert(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"INSERT INTO jobs (job_title) VALUES (?) RETURNING id;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(jobTitle.c_str()), 0, (SQLPOINTER)jobTitle.c_str(), 0, &cbValue), "Bind parameter");
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

bool Job::update(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"UPDATE jobs SET job_title = ? WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(jobTitle.c_str()), 0, (SQLPOINTER)jobTitle.c_str(), 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        return false;
    }

    return true;
}

bool Job::remove(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"DELETE FROM jobs WHERE id = ?;";

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

Job Job::load(HDBC hDBC) {
    RETCODE retCode;
    SQLLEN cbValue = SQL_NTS;
    HSTMT hStmt;
    const int JOB_TITLE_LEN = 25;
    SQLCHAR jobTitle[JOB_TITLE_LEN];
    SQLLEN jobId = 0, jobIdLen = 0, jobTitleLen = 0;

    SQLCHAR* sql = (SQLCHAR*)"SELECT id, job_title FROM jobs WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");

        checkRetCodeException(SQLBindCol(hStmt, 1, SQL_C_USHORT, &jobId, 2, &jobIdLen));
        checkRetCodeException(SQLBindCol(hStmt, 2, SQL_C_CHAR, &jobTitle, JOB_TITLE_LEN, &jobTitleLen));
        checkRetCodeException(SQLFetch(hStmt));
        return Job((long long)jobId, string((char*)jobTitle));
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        cout << "Load job failed: " << exc.retCode << " " << exc.message << endl;
    }

    return Job();
}
