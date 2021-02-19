#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <cstdlib>

class Watchable;

class Session;

class User {
public:
    User(const std::string &name);
    virtual Watchable *getRecommendation(Session &s) = 0;

    std::string getName() const;

    std::vector<Watchable *> get_history() const;
    std::vector<Watchable *> &getHistory();

    void addWatched(Watchable *watched);

    std::map<std::string, int> *get_TagsMap();

    void set_Name(std::string s);

    Watchable *getGenreRec(Session &s);

    bool checkHistory(Watchable *check);

    virtual ~User();

    virtual User *clone()=0;

   void clearHistory();

protected:
    std::vector<Watchable *> history;
private:
    std::string name;
    std::map<std::string, int> TagsMap;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string &name);
    virtual Watchable *getRecommendation(Session &s);
    virtual User *clone();

private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string &name);
    virtual Watchable *getRecommendation(Session &s);
    virtual User *clone();

private:
    int Index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string &name);
    virtual Watchable *getRecommendation(Session &s);
    virtual User *clone();

private:
};

#endif