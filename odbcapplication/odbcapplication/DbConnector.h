#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include<windows.h>
#include<sql.h>
#include<sqlext.h>
#include<iostream>

class DbConnector {
	private:
		HENV hEnv = NULL;
		HDBC hDBC = NULL;
		const char* dataSourceName = "";
		const char* uid = "";
		const char* password = "";
		bool connected = false;

	public:
		DbConnector() {};
		DbConnector(const char* dataSourceName, const char* uid, const char* password);
		void querySql(const char* sql);
		bool connect();
		bool isConnected() {
			return connected;
		}
		HDBC getDBC() {
			return hDBC;
		}
		void close();
		bool checkRetCode(RETCODE retCode);
		void checkRetCodeException(RETCODE retCode);
		int getRowsCount(const char* modelName, std::string search, std::string join);
};

#endif