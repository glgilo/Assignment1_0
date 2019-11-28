#include <iostream>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"

using namespace std;

BaseAction::~BaseAction() {}

BaseAction::BaseAction() {
    status = PENDING;
}
void BaseAction::complete() {
    status = COMPLETED;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::error(const std::string &_errorMsg) {
    status = ERROR;
    errorMsg = _errorMsg;
    cout << errorMsg << endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void CreateUser::act(Session &sess) {
    complete();
    if(sess.getsecond() != "len" && sess.getsecond() != "rer" && sess.getsecond() != "gen"){
        error("Not valid algorithm");
    }
    else if(sess.getuserMap().count(sess.getfirst()) != 0){
        error("The new user name is already taken");
    }

    else if(sess.getsecond() == "len"){
        User *newuse = new LengthRecommenderUser(sess.getfirst());
        sess.addusermap(sess.getfirst(),newuse);
   }
    else if(sess.getsecond() == "rer"){
        User *newuse = new RerunRecommenderUser(sess.getfirst());
        sess.addusermap(sess.getfirst(),newuse);

    }
    else if(sess.getsecond() == "gen"){
        User *newuse = new GenreRecommenderUser(sess.getfirst());
        sess.addusermap(sess.getfirst(),newuse);
    }
}

std::string CreateUser::toString() const {
    return substring("CreateUser");
}

void ChangeActiveUser::act(Session &sess) {
    string userc = sess.getfirst();
    if (sess.getuserMap().count(userc) == 0) {
        error("There is no such user");
    } else {
        User *userTemp = sess.getuserMap().at(userc);
        sess.changeactiveuser(userTemp);
        complete();
    }
}
string ChangeActiveUser::toString() const {
    return substring("ChangeActiveUser");
}

void DeleteUser::act(Session &sess){
    string userToDel = sess.getfirst();
    if (sess.getuserMap().count(userToDel) == 0){
        error("There is no such user");
    }
    else {
        cout << sess.getuserMap().at(userToDel)->getName()<<endl;
        delete sess.getuserMap().at(userToDel);
        sess.deleteUser(userToDel);
        complete();
    }
}

string DeleteUser::toString() const {
    return substring("DeleteUser");
}

void DuplicateUser::act(Session &sess){
    string userNameToDup = sess.getfirst();
    string newUserName = sess.getsecond();
    User *userToDup = sess.getuserMap().at(userNameToDup);
    if (sess.getuserMap().count(userNameToDup) == 0) {
        error("There is no such user");
    }
    else {
        if (userToDup->getAlgoName() == "len") {
            User *newUser = new LengthRecommenderUser(newUserName);
            for (Watchable *content: userToDup->get_history())
                newUser->addtohistory(content);
            sess.addusermap(newUserName, newUser);
        } else if (userToDup->getAlgoName() == "rer") {
            User *newUser = new RerunRecommenderUser(newUserName);
            for (Watchable *content: userToDup->get_history())
                newUser->addtohistory(content);
            sess.addusermap(newUserName, newUser);
        } else if (userToDup->getAlgoName() == "gen") {
            User *newUser = new GenreRecommenderUser(newUserName);
            for (Watchable *content: userToDup->get_history())
                newUser->addtohistory(content);
            sess.addusermap(newUserName, newUser);
        }
    }
}

string DuplicateUser::toString() const {
    return substring("DuplicateUser");
}

void PrintContentList::act(Session &sess) {
    for (Watchable* content: sess.getcontent())
        cout<<content->toString()<<endl;
    complete();
}

string PrintContentList::toString() const {
    return substring("PrintContentList");
}

void PrintWatchHistory::act(Session &sess) {
    vector<Watchable*> history = sess.getActiveUser().get_history();
    if(history.empty())
        cout << "There is no history for " + sess.getActiveUser().getName() << endl;
    else {
        cout << "Watch history for " + sess.getActiveUser().getName() << endl;
        for (int i = 0; i < history.size(); i++) {
            cout << to_string(i + 1) + ". " + history.at(i)->getname() << endl;
        }
    }
}

string PrintWatchHistory::toString() const {
    return substring("PrintWatchHistory");
}

void Watch::act(Session &sess) {
    long id = std::stol(sess.getfirst());
    if(id > 0 && id <= sess.getcontent().size()){
        cout << "watching " + sess.getcontent().at(id-1)->getname() <<endl;
        sess.getActiveUser().addtohistory(sess.getcontent().at(id-1));
        complete();
        Watchable* recommendation = sess.getcontent().at(id-1)->getNextWatchable(sess);
        if (recommendation != nullptr)
            cout << "We recommend watching " + recommendation->getname() ;
        else
            cout << "There is no recommended content available" <<endl;
    }
    else {
        error("no such content");
    }

}

std::string Watch::toString() const {
    return substring("Watch");
}

void PrintActionsLog::act(Session &sess) {
    for (int i = sess.getactionsLog().size()-1; i >= 0; i--){
        cout<<sess.getactionsLog().at(i)->toString() <<endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return substring("PrintActionsLog");
}

std::string BaseAction::substring(std::string action) const {
    if (status == ERROR) {
        return action + " ERROR: " + errorMsg;
    } else if (status == COMPLETED){
        return action + " COMPLETED";
    } else if (status == PENDING)
        return action + " PENDING";
}

void Exit::act(Session &sess) {}

string Exit::toString() const {
    return substring("Exit");
}

BaseAction* CreateUser::clone() const {
    auto* actClone = new CreateUser ();
    setAction(actClone);
    return actClone;
}

BaseAction* ChangeActiveUser::clone() const {
    auto* actClone = new ChangeActiveUser ();
    setAction(actClone);
    return actClone;
}

BaseAction* DeleteUser::clone() const {
    auto* actClone = new DeleteUser ();
    setAction(actClone);
    return actClone;
}

BaseAction* DuplicateUser::clone() const {
    auto* actClone = new DuplicateUser ();
    setAction(actClone);
    return actClone;
}

BaseAction* PrintContentList::clone() const {
    auto* actClone = new PrintContentList ();
    setAction(actClone);
    return actClone;
}

BaseAction* PrintWatchHistory::clone() const {
    auto* actClone = new PrintWatchHistory ();
    setAction(actClone);
    return actClone;
}

BaseAction* Watch::clone() const {
    auto* actClone = new Watch ();
    setAction(actClone);
    return actClone;
}

BaseAction* PrintActionsLog::clone() const {
    auto* actClone = new PrintActionsLog ();
    setAction(actClone);
    return actClone;
}

BaseAction* Exit::clone() const {
    auto* actClone = new Exit ();
    setAction(actClone);
    return actClone;
}

void BaseAction::setAction(BaseAction *actClone) const {
    if (getStatus() == COMPLETED)
        actClone->complete();
    if (getStatus() == ERROR)
        actClone->error(getErrorMsg());
}
