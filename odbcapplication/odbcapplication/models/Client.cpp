#include "Client.h"

Client::Client() {
    this->id = 0;
    this->companyName = "";
    this->postalAddress = "";
    this->phoneNumber = "";
    this->faxNumber = "";
    this->email = "";
}

Client::Client(long long id, string companyName, string postalAddress, string phoneNumber, string faxNumber, string email) {
    this->id = id;
    this->companyName = companyName;
    this->postalAddress = postalAddress;
    this->phoneNumber = phoneNumber;
    this->faxNumber = faxNumber;
    this->email = email;
}

Client::Client(long long id) {
    this->id = id;
    this->companyName = "";
    this->postalAddress = "";
    this->phoneNumber = "";
    this->faxNumber = "";
    this->email = "";
}

int Client::insert(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"INSERT INTO clients (company_name, postal_address, phone_number, fax_number, email) VALUES (?, ?, ?, ?, ?) RETURNING id;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(companyName.c_str()), 0, (SQLPOINTER)companyName.c_str(), 0, &cbValue), "Bind parameter 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(postalAddress.c_str()), 0, (SQLPOINTER)postalAddress.c_str(), 0, &cbValue), "Bind parameter 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(phoneNumber.c_str()), 0, (SQLPOINTER)phoneNumber.c_str(), 0, &cbValue), "Bind parameter 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(faxNumber.c_str()), 0, (SQLPOINTER)faxNumber.c_str(), 0, &cbValue), "Bind parameter 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(email.c_str()), 0, (SQLPOINTER)email.c_str(), 0, &cbValue), "Bind parameter 5");
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

bool Client::update(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"UPDATE clients SET company_name = ?, postal_address = ?, phone_number = ?, fax_number = ?, email = ? WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(companyName.c_str()), 0, (SQLPOINTER)companyName.c_str(), 0, &cbValue), "Bind parameter 1");
        checkRetCodeException(SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(postalAddress.c_str()), 0, (SQLPOINTER)postalAddress.c_str(), 0, &cbValue), "Bind parameter 2");
        checkRetCodeException(SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(phoneNumber.c_str()), 0, (SQLPOINTER)phoneNumber.c_str(), 0, &cbValue), "Bind parameter 3");
        checkRetCodeException(SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(faxNumber.c_str()), 0, (SQLPOINTER)faxNumber.c_str(), 0, &cbValue), "Bind parameter 4");
        checkRetCodeException(SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(email.c_str()), 0, (SQLPOINTER)email.c_str(), 0, &cbValue), "Bind parameter 5");
        checkRetCodeException(SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter 6");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        return false;
    }

    return true;
}

bool Client::remove(HDBC hDBC) {
    HSTMT hStmt;
    SQLLEN cbValue = SQL_NTS;
    SQLCHAR* sql = (SQLCHAR*)"DELETE FROM clients WHERE id = ?;";

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

Client Client::load(HDBC hDBC) {
    RETCODE retCode;
    SQLLEN cbValue = SQL_NTS;
    HSTMT hStmt;
    const int LEN = 25;
    SQLCHAR companyName[LEN], postalAddress[LEN], phoneNumber[LEN], faxNumber[LEN], email[LEN];
    SQLLEN clientId = 0, clientIdLen = 0, len = 0;

    SQLCHAR* sql = (SQLCHAR*)"SELECT id, company_name, postal_address, phone_number, fax_number, email FROM clients WHERE id = ?;";

    try {
        checkRetCodeException(SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt));
        checkRetCodeException(SQLPrepareA(hStmt, sql, SQL_NTS), "Prepare SQL");
        checkRetCodeException(SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 1, 0, (SQLPOINTER)&id, 0, &cbValue), "Bind parameter");
        checkRetCodeException(SQLExecute(hStmt), "Execute stmt");

        checkRetCodeException(SQLBindCol(hStmt, 1, SQL_C_USHORT, &clientId, 2, &clientIdLen));
        checkRetCodeException(SQLBindCol(hStmt, 2, SQL_C_CHAR, &companyName, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 3, SQL_C_CHAR, &postalAddress, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 4, SQL_C_CHAR, &phoneNumber, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 5, SQL_C_CHAR, &faxNumber, LEN, &len));
        checkRetCodeException(SQLBindCol(hStmt, 6, SQL_C_CHAR, &email, LEN, &len));
        checkRetCodeException(SQLFetch(hStmt));
        return Client((long long)clientId, string((char*)companyName), string((char*)postalAddress), string((char*)phoneNumber), string((char*)faxNumber), string((char*)email));
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    catch (ActiveRecordException& exc) {
        cout << "Load clients failed: " << exc.retCode << " " << exc.message << endl;
    }

    return Client();
}
