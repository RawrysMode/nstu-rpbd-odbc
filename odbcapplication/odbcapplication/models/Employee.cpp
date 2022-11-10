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

Employee::Employee(int id, string firstname, string patronymic, string lastname, string dateOfBirth, string residentialAddress, int jobId, int salary) {
    this->id = id;
    this->firstname = firstname;
    this->patronymic = patronymic;
    this->lastname = lastname;
    this->dateOfBirth = dateOfBirth;
    this->residentialAddress = residentialAddress;
    this->jobId = jobId;
    this->salary = salary;
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
    this->jobId = jobTitle.id;
}

int Employee::insert(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"INSERT INTO employees (firstname, patronymic, lastname, date_of_birth, residential_address, job_id, salary) VALUES (?, ?, ?, ?, ?, ?, ?) RETURNING id;";

    /*checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(name.c_str()), 0, (SQLPOINTER)name.c_str(), 0, &cbValue), "Bind parameter");
    checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
    */
    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(firstname.c_str()), 0, (SQLPOINTER)firstname.c_str(), 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(patronymic.c_str()), 0, (SQLPOINTER)patronymic.c_str(), 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(lastname.c_str()), 0, (SQLPOINTER)lastname.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(dateOfBirth.c_str()), 0, (SQLPOINTER)dateOfBirth.c_str(), 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(residentialAddress.c_str()), 0, (SQLPOINTER)residentialAddress.c_str(), 0, &cbValue), "Bind 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&jobId, 0, &cbValue), "Bind 6");
        checkRetCodeException(SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&salary, 0, &cbValue), "Bind 7");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
        if (SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 1, SQL_C_ULONG, &id, 0, NULL);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        cout << exc.message << endl;
        cin.get();


        return 0;
    }

    return id;
}

bool Employee::update(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    //SQLCHAR* sql = (SQLCHAR*)"UPDATE employees SET firstname = ? WHERE id = ?;";
    SQLCHAR* sql = (SQLCHAR*)"UPDATE employees SET firstname = ?, patronymic = ?, lastname = ?, date_of_birth = ?, residential_address = ?, job_id = ?, salary = ? WHERE id = ?;";
    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(firstname.c_str()), 0, (SQLPOINTER)firstname.c_str(), 0, &cbValue), "Bind 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(patronymic.c_str()), 0, (SQLPOINTER)patronymic.c_str(), 0, &cbValue), "Bind 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(lastname.c_str()), 0, (SQLPOINTER)lastname.c_str(), 0, &cbValue), "Bind 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(dateOfBirth.c_str()), 0, (SQLPOINTER)dateOfBirth.c_str(), 0, &cbValue), "Bind 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(residentialAddress.c_str()), 0, (SQLPOINTER)residentialAddress.c_str(), 0, &cbValue), "Bind 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&jobId, 0, &cbValue), "Bind 6");
        checkRetCodeException(SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_NUMERIC, 1, 0, (SQLPOINTER)&salary, 0, &cbValue), "Bind 7");
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

Employee Employee::load(HDBC hDBC) {
    RETCODE retCode;
    SQLLEN cbValue = SQL_NTS;
    HSTMT hStmt;
    const int LEN = 30;
    SQLCHAR firstname[LEN], patronymic[LEN], lastname[LEN], dateOfBirth[LEN], residentialAddress[50];
    SQLLEN employeeId = 0, jobId = 0, salary = 0, len = 0;

    SQLCHAR* sql = (SQLCHAR*)"select id, firstname, patronymic, lastname, date_of_birth, residential_address, job_id, salary from employees WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");

        checkRetCodeException(SQLBindCol(hStmt, 1, SQL_C_LONG, &employeeId, 1, &len));
        checkRetCodeException(SQLBindCol(hStmt, 2, SQL_C_CHAR, &firstname, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 3, SQL_C_CHAR, &patronymic, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 4, SQL_C_CHAR, &lastname, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 5, SQL_C_CHAR, &dateOfBirth, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 6, SQL_C_CHAR, &residentialAddress, 50, &len));
        checkRetCodeException(SQLBindCol(hStmt, 7, SQL_C_LONG, &jobId, 1, &len));
        checkRetCodeException(SQLBindCol(hStmt, 8, SQL_C_LONG, &salary, 1, &len));
        checkRetCodeException(SQLFetch(hStmt));
        return Employee((long long)employeeId, string((char*)firstname), string((char*)patronymic), string((char*)lastname), string((char*)dateOfBirth), string((char*)residentialAddress), (long long)jobId, (long long)salary);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        cout << "Load employees failed: " << exc.retCode << " " << exc.message << endl;
    }

    return Employee();
}