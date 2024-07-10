#include <iostream>
#include "DatabaseManager.h"
#include "MovieDAO.h"
#include "UserDAO.h"
#include "MovieRatingDAO.h"
#include "Services.h"

using namespace std;

void printUsage(const char* programName) {
    cout << "Usage: " << programName << " [--revert]" << endl;
}

#include "DatabaseManager.h"

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

        
            DatabaseManager dbManager(dbName);
            MovieDAO movieDAO(dbManager);
            UserDAO userDAO(dbManager);
            MovieRatingDAO movieRatingDAO(dbManager);

            MovieService movieService(movieDAO);
            UserService userService(userDAO);
            MovieRatingService movieRatingService(movieRatingDAO);

            // 添加电影数据
            movieService.addMovie(1, "Avengers");
            movieService.addMovie(2, "Frozen 2");
            movieService.addMovie(3, "Joker");

            // 添加用户数据
            userService.addUser(1, "Daniel");
            userService.addUser(2, "Monica");
            userService.addUser(3, "Maria");
            userService.addUser(4, "James");

            // 添加电影评分数据
            movieRatingService.addMovieRating(1, 1, 3, "2020-01-12");
            movieRatingService.addMovieRating(1, 2, 4, "2020-02-11");

            // 获取评分最多的用户
            string mostFrequentUser = userService.getMostFrequentUser(movieRatingService.getMovieRatings());
            cout << "Most frequent user: " << mostFrequentUser << endl;

            // 获取2020年2月评分最高的电影
            string topRatedMovie = movieRatingService.getTopRatedMovieInFeb2020(movieService.getMovies());
            cout << "Top rated movie in Feb 2020: " << topRatedMovie << endl;
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

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
