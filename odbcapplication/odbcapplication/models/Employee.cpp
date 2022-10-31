#include "Employee.h"
#include "City.h"

Employee::Employee() {
    this->id = 0;
    this->firstname = "";
    this->patronymic = "";
    this->lastname = "";
    this->dateOfBirth = "";
    this->residentialAddress = "";
    this->setJobTitle(Job());
    this->salary = 0;
}

Employee::Employee(int id) {
    this->id = id;
    this->firstname = "";
    this->patronymic = "";
    this->lastname = "";
    this->dateOfBirth = "";
    this->residentialAddress = "";
    this->setJobTitle(Job());
    this->salary = 0;
}

void Employee::setJobTitle(Job jobTitle) {
    this->jobTitle = jobTitle;
    this->jobTitle = jobTitle.id;
}

int Employee::insert(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"INSERT INTO employees (firstname, patronymic, lastname, date_of_birth, residential_address, job_id, salary) VALUES (?, ?, ?, ?, ?, ?, ?) RETURNING id;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(firstname.c_str()), 0, (SQLPOINTER)&firstname, 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, strlen(patronymic.c_str()), 0, (SQLPOINTER)&patronymic, 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, strlen(lastname.c_str()), 0, (SQLPOINTER)&lastname, 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_DATE, strlen(dateOfBirth.c_str()), 0, (SQLPOINTER)&dateOfBirth, 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, strlen(residentialAddress.c_str()), 0, (SQLPOINTER)&residentialAddress, 0, &cbValue), "Bind 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, 1, 0, (SQLPOINTER)&jobId, 0, &cbValue), "Bind 6");
        checkRetCodeException(SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_NUMERIC, 10, 0, (SQLPOINTER)&salary, 0, &cbValue), "Bind 7");
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

bool Employee::update(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"UPDATE employees SET firstname = ?, patronymic = ?, lastname = ?, date_of_birth = ?, residential_address = ?, job_id = ?, salary = ? WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(firstname.c_str()), 0, (SQLPOINTER)&firstname, 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, strlen(patronymic.c_str()), 0, (SQLPOINTER)&patronymic, 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, strlen(lastname.c_str()), 0, (SQLPOINTER)&lastname, 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_DATE, strlen(dateOfBirth.c_str()), 0, (SQLPOINTER)&dateOfBirth, 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, strlen(residentialAddress.c_str()), 0, (SQLPOINTER)&residentialAddress, 0, &cbValue), "Bind 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, 1, 0, (SQLPOINTER)&jobId, 0, &cbValue), "Bind 6");
        checkRetCodeException(SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_NUMERIC, 10, 0, (SQLPOINTER)&salary, 0, &cbValue), "Bind 7");
        checkRetCodeException(SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        return false;
    }

    return true;
}

bool Employee::remove(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"DELETE FROM employees WHERE id = ?;";

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
