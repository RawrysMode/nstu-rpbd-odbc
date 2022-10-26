#include "ActiveRecord.h"
#include "ActiveRecordException.h"

bool ActiveRecord::save(HDBC hDBC) {
    if (id == 0) {
        id = insert(hDBC);
        return id > 0;
    }

    return update(hDBC);
}

void ActiveRecord::checkRetCodeException(RETCODE retCode) {
    checkRetCodeException(retCode, "SQL result error");
}


void ActiveRecord::checkRetCodeException(RETCODE retCode, std::string comment) {
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
        return;
    }

    throw ActiveRecordException(retCode, comment);
}
