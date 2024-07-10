#include <iostream>
#include <memory>
#include <vector>
#include <sqlite3.h>

using namespace std;

// DBConn 类，用于管理数据库连接
class DBConn {
public:
    // 构造函数，打开数据库连接
    // 连接数据库时，如果失败则输出错误信息并关闭连接
    // 原因: 构造函数负责初始化数据库连接，确保在对象创建时数据库连接已经打开。
    DBConn(const string& db_name) {
        if (sqlite3_open(db_name.c_str(), &db)) {
            cerr << "Can't open database: " << sqlite3_errmsg(db) << '\n';
            sqlite3_close(db);
            db = nullptr;
        } else {
            cout << "DBConn established\n";
        }
    }

    // 析构函数，关闭数据库连接
    // 在对象销毁时，确保数据库连接被正确关闭以释放资源
    // 原因: 析构函数确保在对象销毁时释放资源，防止内存泄漏。
    ~DBConn() {
        if (db) {
            sqlite3_close(db);
            cout << "DBConn closed\n";
        }
    }

    // 执行SQL查询的方法
    // 使用回调函数处理查询结果，确保查询执行成功或输出错误信息
    // 原因: query 方法允许执行任意的 SQL 查询，并通过回调函数处理查询结果或错误信息。
    void query(const string& sql) {
        if (!db) return;

        char* errmsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), callback, 0, &errmsg) != SQLITE_OK) {
            cerr << "SQL error: " << errmsg << '\n';
            sqlite3_free(errmsg);
        } else {
            cout << "Executed query: " << sql << '\n';
        }
    }

private:
    // 回调函数，用于处理查询结果
    // 每次查询结果中的一行数据都会调用该函数，打印每列的名称和值
    // 原因: 回调函数提供了处理查询结果的机制，使得可以在查询完成后处理和显示结果。
    static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
            cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << '\n';
        }
        return 0;
    }

    sqlite3* db = nullptr; // SQLite数据库连接对象
};

// 函数，演示如何使用共享数据库连接
void sharedDatabaseConnection() {
    // 创建一个shared_ptr<DBConn>实例
    // shared_ptr用于共享所有权，使得多个查询对象可以共享同一个数据库连接
    // 原因: 使用shared_ptr确保多个对象可以安全地共享同一个数据库连接，并在最后一个shared_ptr销毁时自动释放资源。
    shared_ptr<DBConn> dbConn = make_shared<DBConn>("test.db");

    vector<shared_ptr<DBConn>> queryObjects;
    for (int i = 0; i < 5; ++i) {
        queryObjects.push_back(dbConn);
        // 每个查询对象都共享同一个数据库连接
        // 原因: 确保每个查询对象使用同一个数据库连接进行查询操作，避免重复创建连接带来的开销。
        queryObjects[i]->query("SELECT sqlite_version();");
    }
    // DBConn 会在最后一个 shared_ptr 销毁时被释放
    // 原因: 使用shared_ptr确保资源在不再使用时正确释放，避免内存泄漏。
}

int main() {
    sharedDatabaseConnection();
    return 0;
}
