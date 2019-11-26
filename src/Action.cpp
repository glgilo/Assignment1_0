#include <iostream>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"

using namespace std;

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
        User *userTemp = sess.getuserMap()[userc];
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
    } else {
        sess.getuserMap()[userToDel]->get_history().clear();
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
    User *userToDup = sess.getuserMap()[userNameToDup];
    if (sess.getuserMap().count(userNameToDup) == 0) {
        error("There is no such user");
    }
    else {
        //User* newUse = sess.getuserMap()[userToDup];
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
    cout << "Watch history for " + sess.getActiveUser().getName() << endl;
    for (int i = 0; i < history.size(); i++){
        cout << to_string(i + 1) + ". " + history.at(i)->getname() <<endl;
    }
}

string PrintWatchHistory::toString() const {
    return substring("PrintWatchHistory");
}

void Watch::act(Session &sess) {
    long id = std::stol(sess.getfirst());
    if(id > 0 && id <= sess.getcontent().size()){
        cout << "watching " + sess.getcontent().at(id-1)->getname() <<endl;
//        Watchable* watch = sess.getcontent().at(id-1);
        sess.getActiveUser().addtohistory(sess.getcontent().at(id-1));
        complete();
        cout << "We recommend watching " + sess.getcontent().at(id-1)->getNextWatchable(sess)->getname() ;
    }
    else {
        error("no such content");
    }

}

std::string Watch::toString() const {
    return substring("Watch");
}

void PrintActionsLog::act(Session &sess) {
//    cout<<sess.getactionsLog().size()<<endl;
    //for(BaseAction* baseaction: sess.getactionsLog()){
    for (int i = sess.getactionsLog().size()-1; i >= 0; i--){
        //cout<<substring(baseaction->toString())<<endl;
        cout<<sess.getactionsLog().at(i)->toString() <<endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return substring("PrintActionsLog");
}

std::string BaseAction::substring(std::string action) const{
    if(status == ERROR){
        return action + " ERROR: " + errorMsg;
    }
    else{
        return action + " COMPLETED";
    }


}