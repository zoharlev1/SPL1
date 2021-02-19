#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;
using namespace std;
class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    Session(const Session& sess);
    void clean();
    void start();
    vector<string> interpreter(string input);
    void insertUser( User* newUser);
    void addActionLog(BaseAction *newAction);
    const vector<string> getMsg ()const;
    bool insertNewUser(CreateUser &act);
    bool changeActiveUser(string userName);
    bool deleteUser(string userName);
    bool duplicate(string origin, string copy, DuplicateUser &act);
    void steal (Session &other);
    bool printContent();
    void printWatchableHistory();
    bool watch(Watch &act);
    void printActionLogHistory();
    Session& operator=(const Session& other);
    void copy(const Session& other);
    Session& operator=(Session&&other);
    Session(Session&& other);
    User& get_activeUser() const;
    vector<Watchable*> get_content() const;
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    vector<string> inputMsg;

};
#endif