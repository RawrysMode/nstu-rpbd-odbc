#include <windows.h>
#include <sql.h>
#include <sqlext.h> 
#include <string.h>
#include <iostream>  
#include <windef.h>
#include "DbConnector.h"
#include "dialog/DialogProcessor.h"
#include "repositories/RepositoryHub.h"

using namespace std;

int main()
{
    string startText = "";
    system("chcp 1251>nul");
    DbConnector dbConnector = DbConnector("PostgreSQL35W", "postgres", "1234");
    if (dbConnector.connect() == true) {
        startText += "Connected to DB.\r\n\r\n";
    }
    else {
        startText += "Connection to PostgreSQL failed. Working without Database.\r\n\r\n";
    }

    RepositoryHub repositoryHub = RepositoryHub(dbConnector);
    repositoryHub.init();

    DialogProcessor dialogProcessor = DialogProcessor(&repositoryHub, startText, dbConnector.isConnected());
    while (!dialogProcessor.isFinished()) {
        dialogProcessor.processRoute();
    }

    cout << endl;
    dbConnector.close();
}
