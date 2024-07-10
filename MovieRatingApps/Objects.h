#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

using namespace std;

class Movie {
public:
    int movie_id;
    string title;

    Movie(int id, const string& t) : movie_id(id), title(t) {}
};

class User {
public:
    int user_id;
    string name;

    User(int id, const string& n) : user_id(id), name(n) {}
};

class MovieRating {
public:
    int movie_id;
    int user_id;
    int rating;
    string created_at;

    MovieRating(int mid, int uid, int r, const string& date) 
        : movie_id(mid), user_id(uid), rating(r), created_at(date) {}
};

#endif // OBJECTS_H
