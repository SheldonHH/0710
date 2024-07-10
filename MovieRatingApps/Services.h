#ifndef SERVICES_H
#define SERVICES_H

#include "MovieDAO.h"
#include "UserDAO.h"
#include "MovieRatingDAO.h"
#include "Objects.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

using namespace std;

class MovieService {
private:
    MovieDAO& movieDAO;
    vector<shared_ptr<Movie>> movies;

public:
    MovieService(MovieDAO& movieDAO) : movieDAO(movieDAO) {}

    void addMovie(int id, const string& title) {
        movieDAO.addMovie(id, title);
        movies.push_back(make_shared<Movie>(id, title));
    }

    vector<shared_ptr<Movie>> getMovies() const {
        return movies;
    }
};

class UserService {
private:
    UserDAO& userDAO;
    vector<shared_ptr<User>> users;

public:
    UserService(UserDAO& userDAO) : userDAO(userDAO) {}

    void addUser(int id, const string& name) {
        userDAO.addUser(id, name);
        users.push_back(make_shared<User>(id, name));
    }

    string getMostFrequentUser(const vector<shared_ptr<MovieRating>>& movieRatings) const {
        unordered_map<int, int> userRatingCount;
        for (const auto& rating : movieRatings) {
            userRatingCount[rating->user_id]++;
        }

        auto maxElement = max_element(userRatingCount.begin(), userRatingCount.end(),
            [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.second < b.second;
            });

        for (const auto& user : users) {
            if (user->user_id == maxElement->first) {
                return user->name;
            }
        }
        return "";
    }
};

class MovieRatingService {
private:
    MovieRatingDAO& movieRatingDAO;
    vector<shared_ptr<MovieRating>> movieRatings;

public:
    MovieRatingService(MovieRatingDAO& movieRatingDAO) : movieRatingDAO(movieRatingDAO) {}

    void addMovieRating(int movie_id, int user_id, int rating, const string& date) {
        movieRatingDAO.addMovieRating(movie_id, user_id, rating, date);
        movieRatings.push_back(make_shared<MovieRating>(movie_id, user_id, rating, date));
    }

    vector<shared_ptr<MovieRating>> getMovieRatings() const {
        return movieRatings;
    }

    string getTopRatedMovieInFeb2020(const vector<shared_ptr<Movie>>& movies) const {
        unordered_map<int, vector<int>> movieRatingsMap;
        for (const auto& rating : movieRatings) {
            if (rating->created_at.substr(0, 7) == "2020-02") {
                movieRatingsMap[rating->movie_id].push_back(rating->rating);
            }
        }

        double highestAvgRating = 0;
        int topMovieId = -1;

        for (const auto& [movieId, ratings] : movieRatingsMap) {
            double avgRating = accumulate(ratings.begin(), ratings.end(), 0.0) / ratings.size();
            if (avgRating > highestAvgRating) {
                highestAvgRating = avgRating;
                topMovieId = movieId;
            }
        }

        for (const auto& movie : movies) {
            if (movie->movie_id == topMovieId) {
                return movie->title;
            }
        }
        return "";
    }
};

#endif // SERVICES_H
