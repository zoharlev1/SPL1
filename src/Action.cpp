#include"../include/json.hpp"
#include"../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include<fstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;


BaseAction::BaseAction() : errorMsg(""), status(PENDING) {};

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::setError(std::string error) {
    errorMsg = error;
}

void BaseAction::setStatus() {
    complete();
}

BaseAction::~BaseAction() {

}

void BaseAction::setStatus(ActionStatus curr) {
    this->status = curr;
}

//Create user class------------------------------------------------------------------
void CreateUser::act(Session &sess) {
    if (sess.getMsg().size() != 3) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        if (!sess.insertNewUser(*this)) {
            CreateUser::error(getErrorMsg());
            cout << "ERROR " + getErrorMsg() << endl;
        } else { complete(); }
        sess.addActionLog(this);
    }
}

std::string CreateUser::toString() const {
    string output = "";
    if (getStatus() == COMPLETED) {
        output = "CreateUser COMPLETED";
    } else { output = "CreateUser ERROR: " + getErrorMsg(); }
    return output;
}

//
/////Createuser clone
BaseAction *CreateUser::clone() {
    CreateUser *actCreate = new CreateUser();
    actCreate->setError(this->getErrorMsg());
    actCreate->setStatus(this->getStatus());
    return actCreate;
}


//Change Active user--------------------------------------------------------------
void ChangeActiveUser::act(Session &sess) {
    if (sess.getMsg().size() != 2) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        if (!sess.changeActiveUser(sess.getMsg().at(1))) {
            ChangeActiveUser::error("username does not exist");
            cout << "ERROR " + getErrorMsg() << endl;
        } else { complete(); }
        sess.addActionLog(this);
    }
}

std::string ChangeActiveUser::toString() const {
    string output = "";
    if (getStatus() == COMPLETED) {
        output = "ChangedUser COMPLETED";
    } else { output = "ChangeUser ERROR: " + getErrorMsg(); }
    return output;
}

/////Change user clone
BaseAction *ChangeActiveUser::clone() {
    ChangeActiveUser *actChange = new ChangeActiveUser();
    actChange->setError(this->getErrorMsg());
    actChange->setStatus(this->getStatus());
    return actChange;
}

//Delete user----------------------------------------------------------------------
void DeleteUser::act(Session &sess) {
    if (sess.getMsg().size() != 2) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        if (!sess.deleteUser(sess.getMsg().at(1))) {
            DeleteUser::error("the user is not exist- so can't be deleted");
            cout << "ERROR " + getErrorMsg() << endl;
        } else { complete(); }
        sess.addActionLog(this);
    }
}
std::string DeleteUser::toString() const {
    string output = "";
    if (getStatus() == COMPLETED) {
        output = "DeleteUser COMPLETED";
    } else { output = "DeleteUser ERROR: " + getErrorMsg(); }

    return output;
}

/////Delete user clone
BaseAction *DeleteUser::clone() {
    DeleteUser *actDelete = new DeleteUser();
    actDelete->setError(this->getErrorMsg());
    actDelete->setStatus(this->getStatus());
    return actDelete;
}


//Duplicate user----------------------------------------------------------------------
void DuplicateUser::act(Session &sess) {
    if (sess.getMsg().size() != 3) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        if (!sess.duplicate(sess.getMsg()[1], sess.getMsg()[2], *this)) {
            BaseAction::error(getErrorMsg());
            cout << "ERROR " + getErrorMsg() << endl;
        } else {
            complete();
        }
        sess.addActionLog(this);
    }
}

std::string DuplicateUser::toString() const {
    string output = "";
    if (getStatus() == COMPLETED) {
        output = "DuplicateUser COMPLETED";
    } else { output = "DuplicateUser ERROR: " + getErrorMsg(); }
    return output;
}

/////Duplicate user clone
BaseAction *DuplicateUser::clone() {
    DuplicateUser *actDup = new DuplicateUser();
    actDup->setError(this->getErrorMsg());
    actDup->setStatus(this->getStatus());
    return actDup;
}

//Print content watchable list-----------------------------------------------------------
void PrintContentList::act(Session &sess) {
    if (sess.getMsg().size() != 1) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        if (!sess.printContent()) {
            PrintContentList::error("the watchable content is empty");
            cout << "ERROR " + getErrorMsg() << endl;
        } else { complete(); }
        sess.addActionLog(this);
    }
}
std::string PrintContentList::toString() const {
    string output = "";
    if (getStatus() == COMPLETED) {
        output = "PrintContentList COMPLETED";
    } else { output = "PrintContentList ERROR: " + getErrorMsg(); }
    return output;
}

/////PrintContentList user clone
BaseAction *PrintContentList::clone() {
    PrintContentList *actPrintCon = new PrintContentList();
    actPrintCon->setError(this->getErrorMsg());
    actPrintCon->setStatus(this->getStatus());
    return actPrintCon;
}

//Print watch history--------------------------------------------------------------------
void PrintWatchHistory::act(Session &sess) {
    if (sess.getMsg().size() != 1) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        sess.printWatchableHistory();
        complete();
        sess.addActionLog(this);
    }
}
std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory COMPLETED";
}

////PrintWatchHistory user clone
BaseAction *PrintWatchHistory::clone() {
    PrintWatchHistory *actPrintHis = new PrintWatchHistory();
    actPrintHis->setError(this->getErrorMsg());
    actPrintHis->setStatus(this->getStatus());
    return actPrintHis;
}

//Print Actions Log------------------------------
void PrintActionsLog::act(Session &sess) {
    if (sess.getMsg().size() != 1) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        sess.printActionLogHistory();
        complete();
        sess.addActionLog(this);
    }
}
std::string PrintActionsLog::toString() const {
    return "PrintActionsLog COMPLETED";
}

////PrintActionsLog user clone
BaseAction *PrintActionsLog::clone() {
    PrintActionsLog *actPrintLog = new PrintActionsLog();
    actPrintLog->setError(this->getErrorMsg());
    actPrintLog->setStatus(this->getStatus());
    return actPrintLog;
}

//Watch -----------------------------------
void Watch::act(Session &sess) {
    if (sess.getMsg().size() != 2) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        if (!sess.watch(*this)) {
            Watch::error(getErrorMsg());
            cout << "ERROR " + getErrorMsg() << endl;
        } else {
            complete();
        }
        sess.addActionLog(this);
    }
}

///Watch toString -------
std::string Watch::toString() const {
    string output = "";
    if (getStatus() == COMPLETED) {
        output = "Watch COMPLETED";
    } else { output = "ERROR: " + getErrorMsg(); }

    return output;
}

////Watch user clone
BaseAction *Watch::clone() {
    Watch *actWatch = new Watch();
    actWatch->setError(this->getErrorMsg());
    actWatch->setStatus(this->getStatus());
    return actWatch;
}


//class Exit-----------------------------------------------
void Exit::act(Session &sess) {
    if (sess.getMsg().size() != 1) {
        setError("wrong syntax");
        cout << "ERROR " + getErrorMsg() << endl;
    } else {
        complete();
        sess.addActionLog(this);
    }
}
std::string Exit::toString() const {
    return "EXIT COMPLETED";
}

////Exit user clone
BaseAction *Exit::clone() {
    Exit *actExit = new Exit();
    actExit->setError(this->getErrorMsg());
    actExit->setStatus(this->getStatus());
    return actExit;
}


