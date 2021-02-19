#include <utility>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <cstdlib>
#include <bits/stdc++.h>
#include  <cmath>

using namespace std;

////User Constructor
User::User(const std::string &name) : history(), name(name), TagsMap() {}


////User getName method
string User::getName() const {
    return name;
}

///User get_TagsMap
std::map<std::string, int> *User::get_TagsMap() {
    return &TagsMap;
}

///User Set_name
void User::set_Name(std::string s) {
    name = std::move(s);
}

////User get_history method
vector<Watchable *> User::get_history() const {
    return history;
}

///User addWatched
void User::addWatched(Watchable *watched) {
    history.push_back(watched);
    //for 3rd algorithm
    vector<string> currTags = watched->get_tags();
    for (const auto &x: currTags) {
        TagsMap[x]++;
    }
}

///User Destructor
User::~User() {}

std::vector<Watchable *>& User::getHistory() {
    return history;
}

////class LengthRecommenderUser--------------------------------------------
/// LengthRecommenderUser constructor
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}

///LengthUser clone for deep copy
User *LengthRecommenderUser::clone() {
    LengthRecommenderUser *copy = new LengthRecommenderUser(this->getName());
    for (auto m: *get_TagsMap()) {
        copy->get_TagsMap()->insert({m.first, m.second});
    }
    for(auto x: history)
    {
        Watchable* temp = x;
        copy->history.push_back(temp);

    }

    return copy;
}
/// LengthRecommenderUser getRecommendation virtual method

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    Watchable *output = nullptr;
    User &currUser = s.get_activeUser();
    vector<Watchable *> currHistory = currUser.get_history();
    //if the user has just finished watching an episode we need to recommend the next episode if there is one
    if (currHistory[currHistory.size() - 1]->getNextWatchable(s) != nullptr) {
        return currHistory[currHistory.size() - 1]->getNextWatchable(s);
    }


    //finding user average length
    float lengthaAcumulator = 0;
    float counter = 0;
    for (Watchable *x : currHistory) {
        int currLength = x->get_length();
        lengthaAcumulator = lengthaAcumulator + currLength;
        counter++;
    }
    float averageLength = lengthaAcumulator / counter;

    vector<Watchable *> currContent = s.get_content();
    float minLengthDifference = MAXFLOAT;
    bool watched = false;
    for (Watchable *nextWatchCanidate : currContent) {
        int currLength = nextWatchCanidate->get_length();
        if (abs(averageLength - currLength) < minLengthDifference) {
            for (size_t i = 0; i < currHistory.size() && !watched; i++) {
                if (currHistory[i]->get_id() == nextWatchCanidate->get_id()) {
                    watched = true;
                }
            }
            if (!watched) {
                minLengthDifference = abs(averageLength - currLength);
                output = nextWatchCanidate;
            }
            watched = false;
        }
    }

    return output;
}


///-----------------------------------------------------------------------

///class RerunRecommenderUser----------------------------------------------
///RerunRecommenderUser constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), Index(-1) {}

///Rerun Clone
User *RerunRecommenderUser::clone() {
    auto *copy = new RerunRecommenderUser(this->getName());
    for (auto m: *get_TagsMap()) {
        copy->get_TagsMap()->insert({m.first, m.second});
    }
    for(auto x: history)
    {
        Watchable* temp = x;
        copy->history.push_back(temp);
    }
    return copy;
}

///RerunRecommenderUser getRecommendation virtual method
Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    User &currUser = s.get_activeUser();
    vector<Watchable *> currHistory = currUser.get_history();
    //if the user has just finished watching an episode we need to recommend the next episode if there is one
    if (currHistory[currHistory.size() - 1]->getNextWatchable(s) != nullptr) {
        return currHistory[currHistory.size() - 1]->getNextWatchable(s);
    }
    Index++;
    return currHistory[Index % currHistory.size()];


}

///-------------------------------------------------------------------------------
///class GenreRecommenderUser-----------------------------------------------------
/// GenreRecommenderUser constructor
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {}

///clone for deep copy
User *GenreRecommenderUser::clone() {
    auto *copy = new GenreRecommenderUser(this->getName());
    for (auto m: *get_TagsMap()) {
        copy->get_TagsMap()->insert({m.first, m.second});
    }
    for(auto x: history)
    {
        Watchable* temp = x;
        copy->history.push_back(temp);
    }
    return copy;
}

///GenreRecommenderUser getRecommendation virtual method
Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    Watchable *output = nullptr;
    User &currUser = s.get_activeUser();
    vector<Watchable *> currHistory = currUser.get_history();
    //if the user has just finished watching an episode we need to recommend the next episode if there is one
    if (currHistory[currHistory.size() - 1]->getNextWatchable(s) != nullptr) {
        return currHistory[currHistory.size() - 1]->getNextWatchable(s);
    }

    output = getGenreRec(s);
    return output;
}

///get GenreRecommendation
Watchable *User::getGenreRec(Session &s) {
    map<string, int> *Tmap = get_TagsMap();
    map<string, int>::iterator itr;
    int MaxTagCount = 0;
    for (itr = Tmap->begin(); itr != Tmap->end(); ++itr) {
        if (itr->second > MaxTagCount) {
            MaxTagCount = itr->second;
        }
    }
    string maxTag = "";
    bool foundMax = false;
    for (itr = Tmap->begin(); itr != Tmap->end() && !foundMax; ++itr) {
        if (itr->second == MaxTagCount) {
            maxTag = itr->first;
            foundMax = true;
        }
    }
    if(maxTag == ""){
        return nullptr;
    }
    //bool foundinhist = false;
    for (size_t i = 0; i < s.get_content().size(); i++) {
        for (size_t k = 0; k < s.get_content()[i]->get_tags().size(); k++) {
            if (s.get_content()[i]->get_tags()[k] == maxTag) {
                bool p = checkHistory(s.get_content()[i]);
                if (!p) {
                    return s.get_content()[i];
                }
            }

        }
    }
    Tmap->erase(maxTag);
    return getGenreRec(s);
}

bool User::checkHistory(Watchable *check) {
    bool found = false;
    for (size_t z = 0; z < history.size() && !found; z++) {
        if (history[z]->get_id() == check->get_id()) {
            found = true;
        }
    }
    return found;
}

void User::clearHistory() {
    while(!history.empty()){
        history.pop_back();
    }


}









