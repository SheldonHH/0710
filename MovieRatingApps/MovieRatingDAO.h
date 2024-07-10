#ifndef MOVIERATINGDAO_H
#define MOVIERATINGDAO_H

#include "DatabaseManager.h"
#include <string>

using namespace std;

class MovieRatingDAO {
private:
    DatabaseManager& dbManager;

public:
    MovieRatingDAO(DatabaseManager& dbManager) : dbManager(dbManager) {}

    void addMovieRating(int movie_id, int user_id, int rating, const string& date) {
        string sql = "INSERT INTO MovieRating (movie_id, user_id, rating, created_at) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(dbManager.getDb(), sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, movie_id);
            sqlite3_bind_int(stmt, 2, user_id);
            sqlite3_bind_int(stmt, 3, rating);
            sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw runtime_error("Error inserting data: " + string(sqlite3_errmsg(dbManager.getDb())));
            }
            sqlite3_finalize(stmt);
        } else {
            throw runtime_error("Error preparing statement: " + string(sqlite3_errmsg(dbManager.getDb())));
        }
    }
};

#endif // MOVIERATINGDAO_H
