#include "DbConnector.h"
#include "models/City.h"

DbConnector::DbConnector(const char* dataSourceName, const char* uid, const char* password) {
    this->dataSourceName = dataSourceName;
    this->uid = uid;
    this->password = password;
}

bool DbConnector::checkRetCode(RETCODE retCode) {
    return retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO;
}

void DbConnector::checkRetCodeException(RETCODE retCode) {
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
        return;
    }

    throw "SQL result error";
}

bool DbConnector::connect() {
    RETCODE retCode;
    retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    connected = true;

    if (!checkRetCode(retCode)) {
        std::cout << "Error allocating environment descriptor." << std::endl;
        return connected = false;
    }

    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    if (!checkRetCode(retCode)) {
        std::cout << "Error setting ODBC version." << std::endl;
        return connected = false;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);
    if (!checkRetCode(retCode)) {
        std::cout << "Error applying connection descriptor." << std::endl;
        return connected = false;
    }


    retCode = SQLConnectA(hDBC, (SQLCHAR*)dataSourceName, SQL_NTS, (SQLCHAR*)uid, SQL_NTS, (SQLCHAR*)password, SQL_NTS);
    if (!checkRetCode(retCode)) {
        std::cout << "Error connecting to the DB using specified DSN, UID and password." << std::endl;
        return connected = false;
    }

    return connected;
}

void DbConnector::querySql(const char* sql) {
    if (!connected) {
        return;
    }

    HSTMT hStmt;
    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLExecDirectA(hStmt, (SQLCHAR*)sql, SQL_NTS));
    }
    catch (const char* msg) {}

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DbConnector::close() {
    if (!connected) {
        return;
    }

    SQLFreeHandle(SQL_HANDLE_ENV, &hEnv);
    SQLDisconnect(hDBC);
    SQLFreeHandle(SQL_HANDLE_DBC, &hDBC);
    std::cout << "DB connection closed." << std::endl;
}

int DbConnector::getRowsCount(const char* tableName, std::string search, std::string join) {
    HSTMT hStmt;
    SQLLEN rowsCount = 0;
    SQLLEN rowsCountLen = 0;
    std::string sql = "SELECT count(t1.*) FROM ";
    sql.append(tableName);
    sql.append(" AS t1 ");

    if (!join.empty()) {
        sql.append(join);
    }

    if (!search.empty()) {
        sql.append(" " + search + " ");
    }
    sql.append(";");

    SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);
    try {
        checkRetCodeException(SQLExecDirectA(hStmt, (SQLCHAR*)sql.c_str(), SQL_NTS));
        checkRetCodeException(SQLBindCol(hStmt, 1, SQL_C_SBIGINT, &rowsCount, 0, &rowsCountLen));
        SQLFetch(hStmt);
    }
    catch (const std::exception& exc) {
        std::cout << exc.what() << std::endl;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return rowsCount;
}
