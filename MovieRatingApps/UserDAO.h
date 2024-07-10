#ifndef USERDAO_H
#define USERDAO_H

#include "DatabaseManager.h"
#include <string>

using namespace std;

class UserDAO {
private:
    DatabaseManager& dbManager;

public:
    UserDAO(DatabaseManager& dbManager) : dbManager(dbManager) {}

    void addUser(int id, const string& name) {
        string sql = "INSERT INTO Users (user_id, name) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(dbManager.getDb(), sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw runtime_error("插入数据错误: " + string(sqlite3_errmsg(dbManager.getDb())));
            }
            sqlite3_finalize(stmt);
        } else {
            throw runtime_error("准备语句错误: " + string(sqlite3_errmsg(dbManager.getDb())));
        }
    }
};

#endif // USERDAO_H
