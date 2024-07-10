#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace sql;

class DatabaseManager {
private:
    unique_ptr<Connection> conn;

public:
    DatabaseManager(const string& host, const string& user, const string& pass, const string& db_name) {
        try {
            mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
            conn.reset(driver->connect(host, user, pass));
            conn->setSchema(db_name);
        } catch (SQLException& e) {
            cerr << "Error: " << e.what() << endl;
            throw runtime_error("无法连接到数据库");
        }
    }

    void createTables() const {
        const string sql = R"(
            CREATE TABLE IF NOT EXISTS Movies (
                movie_id INT PRIMARY KEY,
                title VARCHAR(255) NOT NULL
            );
            CREATE TABLE IF NOT EXISTS Users (
                user_id INT PRIMARY KEY,
                name VARCHAR(255) NOT NULL
            );
            CREATE TABLE IF NOT EXISTS MovieRating (
                movie_id INT,
                user_id INT,
                rating INT,
                created_at DATE,
                PRIMARY KEY (movie_id, user_id),
                FOREIGN KEY (movie_id) REFERENCES Movies(movie_id),
                FOREIGN KEY (user_id) REFERENCES Users(user_id)
            );
        )";
        executeSQL(sql);
    }

    void dropTables() const {
        const string sql = R"(
            DROP TABLE IF EXISTS MovieRating;
            DROP TABLE IF EXISTS Movies;
            DROP TABLE IF EXISTS Users;
        )";
        executeSQL(sql);
    }

private:
    void executeSQL(const string& sql) const {
        try {
            unique_ptr<Statement> stmt(conn->createStatement());
            stmt->execute(sql);
            cout << "Operation executed successfully." << endl;
        } catch (SQLException& e) {
            cerr << "SQL error: " << e.what() << endl;
        }
    }
};

void printUsage(const char* programName) {
    cout << "Usage: " << programName << " [--revert]" << endl;
}

int main(int argc, char* argv[]) {
    string dbHost = "tcp://127.0.0.1:3306";
    string dbUser = "root";
    string dbPass = "password";
    string dbName = "movies_db";
    bool revert = false;

    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--revert") {
            revert = true;
        } else if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
    }

    try {
        DatabaseManager dbManager(dbHost, dbUser, dbPass, dbName);

        if (revert) {
            cout << "This will drop all tables and recreate them. Type 'YES' to confirm: ";
            string input;
            cin >> input;
            if (input == "YES") {
                dbManager.dropTables();
                dbManager.createTables();
            } else {
                cout << "Operation cancelled." << endl;
            }
        } else {
            dbManager.createTables();
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
