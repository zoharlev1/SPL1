#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include <fstream>
#include <sstream>
#include "../include/User.h"

using namespace std;

///Destructor
Session::~Session() {
    clean();
}

///copy constructor
Session::Session(const Session &sess) : content(), actionsLog(), userMap(), activeUser(), inputMsg() {
    copy(sess);
}

///copy assignment operator
Session &Session::operator=(const Session &other) {
    if (&other != this) {
        clean();
        copy(other);
    }

    return *this;
}


Session::Session(const std::string &configFilePath) : content(), actionsLog(), userMap(), activeUser(), inputMsg() {
    activeUser = new LengthRecommenderUser("default");
   insertUser(activeUser);
    using json = nlohmann::json;
    json j;
    std::ifstream ifs(configFilePath);
    ifs >> j;
    ifs.close();

    //inserting movies into content
    long Id = 1;
    json movies = j["movies"];
    for (auto &x : movies.items()) {
        json movie = x.value();
        string currMovieName = movie["name"];
        int currMovieLength = movie["length"];

        //making tags in to string vector tags
        vector<string> currMovietags;
        for (auto &y : movie["tags"]) {
            string currTag = y;
            currMovietags.push_back(currTag);

        }
        auto *currMovie = new Movie(Id, currMovieName, currMovieLength, currMovietags);
        content.push_back(currMovie);
        Id++;
    }

    //inserting Episodes into content

    json series = j["tv_series"];
    for (auto &x : series.items()) {
        json tv_Show = x.value();
        string currtvShowName = tv_Show["name"];
        int currtvShowLength = tv_Show["episode_length"];
        //making tags in to string vector tags
        vector<string> currtvShowtags;
        for (auto &y : tv_Show["tags"]) {
            string currTag = y;
            currtvShowtags.push_back(currTag);

        }

        int seasonCounter = 1;
        Episode *currEpisode;
        for (auto &season : tv_Show["seasons"]) {
            for (int i = 1; i <= season; i++) {
                currEpisode = new Episode(Id, currtvShowName, currtvShowLength, seasonCounter, i, currtvShowtags);
                content.push_back(currEpisode);
                Id++;
                //set next episode to id+1
                currEpisode->set_nextEpisodeId(long(Id));
            }
            seasonCounter++;
        }
        //setting next episode_id to null
        currEpisode->set_nextEpisodeId(long(0));
    }
}

void Session::start() {
    std::cout << "SPLflix is now on!" << std::endl;
    string input = "";
    inputMsg.push_back("");
    while (inputMsg.at(0) != "exit") {
        getline(cin, input);
        inputMsg = interpreter(input);
        if (inputMsg.at(0) == "createuser") {
            CreateUser *actCreate = new CreateUser();
            actCreate->act(*this);
        }
        else if (inputMsg.at(0) == "changeuser") {
            ChangeActiveUser *changeUser = new ChangeActiveUser();
            changeUser->act(*this);
        }
        else if (inputMsg.at(0) == "deleteuser") {
            DeleteUser *deleteUser = new DeleteUser();
            deleteUser->act(*this);
        }
        else if (inputMsg.at(0) == "dupuser") {
            DuplicateUser *dupUser = new DuplicateUser();
            dupUser->act(*this);
        }
        else if (inputMsg.at(0) == "content") {
            PrintContentList *contentList = new PrintContentList();
            contentList->act(*this);
        }
        else if (inputMsg.at(0) == "watchhist") {
            PrintWatchHistory *watchList = new PrintWatchHistory();
            watchList->act(*this);
        }
        else if (inputMsg.at(0) == "watch") {
            Watch *watch = new Watch();
            watch->act(*this);
        }
        else if (inputMsg.at(0) == "log") {
            PrintActionsLog *actionLog = new PrintActionsLog();
            actionLog->act(*this);
        } else if(inputMsg.at(0)!="exit"){
            cout << "invalid input" << endl;
        }
    }
    Exit *exitAct = new Exit();
    exitAct->act(*this);
    inputMsg.pop_back();
}


////analyzing the user's input
vector<string> Session::interpreter(string input) {
    vector<string> strV;
    istringstream iss(input);
    vector<string> results((istream_iterator<string>(iss)),
                           istream_iterator<string>());
    for (auto x: results) {
        strV.push_back(x);
    }
    return strV;
}

/// get_activeUser Method
User &Session::get_activeUser() const {
    return *activeUser;
}

/// get content vector method
vector<Watchable *> Session::get_content() const {
    return content;
}


const vector<string> Session::getMsg() const {
    return inputMsg;
}

void Session::insertUser(User *newUser) {
    userMap.insert({newUser->getName(), newUser});
}


void Session::addActionLog(BaseAction *newAction) {
    actionsLog.push_back((BaseAction *const) newAction);
}

bool Session::insertNewUser(CreateUser &act) {
    string userName = getMsg().at(1);
    string algo = getMsg().at(2);
    if (!(userMap.find(userName) == userMap.end())) {
        act.setError("the user name is already taken");
        return false;
    } else {
        if (algo == "len") {
            LengthRecommenderUser *inputUser = new LengthRecommenderUser(userName);
            Session::insertUser(inputUser);
        } else if (algo == "rer") {
            RerunRecommenderUser *inputUser = new RerunRecommenderUser(userName);
            Session::insertUser(inputUser);
        } else if (algo == "gen") {
            GenreRecommenderUser *inputUser = new GenreRecommenderUser(userName);
            Session::insertUser(inputUser);
        } else {
            act.setError("recommendation algorithm is invalid");
            return false;
        }
        return true;
    }
}

bool Session::changeActiveUser(string userName) {
    unordered_map<std::string, User *>::iterator iter;
    iter = userMap.find(userName);
    if (iter == userMap.end()) {
        return false;
    } else {
        if (activeUser->getName() == userName)
            return true;
        activeUser = iter->second;
    }
    return true;
}

bool Session::deleteUser(string userName) {
    unordered_map<std::string, User *>::iterator iter;
    iter = userMap.find(userName);
    if (iter == userMap.end()) {
        return false;
    } else {
        User *temp = userMap[userName];
        userMap.erase(userName);
        delete temp;
    }

    return true;
}

bool Session::duplicate(string origin, string copy, DuplicateUser &act) {

    unordered_map<std::string, User *>::iterator iter;
    iter = userMap.find(origin);
    if (iter == userMap.end()) {
        act.setError("username does not exist");
        return false;
    } else if (iter->first == copy) {
        act.setError("username is already in use");
        return false;
    } else {
        User *NewUser = iter->second->clone();
        NewUser->set_Name(copy);
        insertUser(NewUser);
        return true;
    }
}


bool Session::printContent() {
    if (content.size() == 0) {
        return false;
    } else {
        for (auto x: content) {
            cout << x->toString() << endl;
        }
        return true;
    }
}

void Session::printWatchableHistory() {
    cout << "Watch history for " + activeUser->getName() << endl;
    for (Watchable *x : activeUser->get_history()) {
        cout << x->printString() << endl;
    }
}

void Session::printActionLogHistory() {
    for (int i = actionsLog.size() - 1; i >= 0; i--) {
        cout << actionsLog.at(i)->toString() << endl;
    }
}

///Watch Funtion------
bool Session::watch(Watch &act) {
    stringstream x(getMsg().at(1));
    int CurrId = 0;
    x >> CurrId;
    if (x.fail()) {
        act.setError("wrong syntax");
        return false;
    }
    int z = get_content().size() - 1;
    if (CurrId > z) {
        act.setError("content Id does not exist");
        return false;
    } else {
        cout << "Watching";
        std::string s = content[CurrId - 1]->printString();
        cout << s.substr(2) << endl;
        activeUser->addWatched(content[CurrId - 1]);
        Watchable *currRecom = activeUser->getRecommendation(*this);
        if(currRecom==nullptr) {
            cout << "No more recommendations" << endl;
            return true;
        }
        cout << "We recommend watching " + currRecom->printString().substr(3) + " continue watching? [y/n]" << endl;
        string input = "";
        getline(cin, input);
        while (input == "y") {
            cout << "Watching";
            std::string s = content[currRecom->get_id() - 1]->printString();
            cout << s.substr(2) << endl;
            activeUser->addWatched(currRecom);
            currRecom = activeUser->getRecommendation(*this);
            if(currRecom==nullptr)
            {
                cout<<"No more recommendations"<<endl;
                input = "n";
            }else {
                cout << "We recommend watching " + currRecom->printString().substr(3) + " continue watching? [y/n]"
                     << endl;
                input = "";
                getline(cin, input);
                Watch *curr = new Watch;
                curr->setStatus();
                this->addActionLog(curr);
            }
        }
        return true;
    }
}

void Session::clean() {
    for (auto x: content) {
        delete (x);
        x = nullptr;
    }
    content.clear();

    for (auto y: actionsLog) {
        delete (y);
        y = nullptr;
    }
    actionsLog.clear();
    for (pair<string, User *> z: userMap) {
        delete (z.second);
        z.second = nullptr;
    }

    userMap.clear();
    inputMsg.clear();
}

void Session::copy(const Session &other) {
    for (size_t i = 0; i < other.content.size(); i++) {
        content.push_back(other.content[i]->clone());
    }
    for (size_t i = 0; i < other.actionsLog.size(); i++) {
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for (auto mapIter = other.userMap.begin(); mapIter != other.userMap.end(); ++mapIter) {
        User *newUser = mapIter->second->clone();
        newUser->clearHistory();
        for (size_t z = 0; z < other.userMap.find(newUser->getName())->second->getHistory().size(); z++) {
            newUser->addWatched(
                    content.at(other.userMap.find(newUser->getName())->second->getHistory()[z]->get_id() - 1));
        }

        userMap.insert({newUser->getName(), newUser});
    }
    activeUser = userMap[other.get_activeUser().getName()];
}

/// move constructor
Session::Session(Session &&other) : content(), actionsLog(), userMap(), activeUser(), inputMsg() {
    this->clean();
    steal(other);
}

void Session::steal(Session &other) {
    for (size_t i = 0; i < other.content.size(); i++) {
        Watchable *newPtr = other.content[i];
        content.push_back(newPtr);
        other.content[i] = nullptr;
    }
    for (size_t i = 0; i < other.actionsLog.size(); i++) {
        this->actionsLog.push_back(other.actionsLog[i]);
        other.actionsLog[i] = nullptr;
    }
    for (pair<string, User *> x: other.userMap) {
        userMap.insert({x.second->getName(), x.second});
    }
    other.userMap = unordered_map<string, User *>();

    User *temp = other.activeUser;
    activeUser = temp;
    other.activeUser = nullptr;
}

///move assignment operator
Session &Session::operator=(Session &&other) {
    if (this != &other) {
        this->clean();
        steal(other);
    }
    return *this;
}






