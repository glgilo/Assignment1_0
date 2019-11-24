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

void CreateUser::act(Session &sess) {
    complete();
    if(sess.getsecond() != "len" && sess.getsecond() != "rer" && sess.getsecond() != "gen"){
        error("Not valid algorithm");
    }
    else if(sess.getuserMap().count(sess.getfirst()) != 0){
        error("The new user name is already taken");
    }

    if(sess.getsecond() == "len"){
        User *newuse = new LengthRecommenderUser(sess.getfirst());
        sess.addusermap(sess.getfirst(),newuse);
   }
    if(sess.getsecond() == "rer"){
        User *newuse = new RerunRecommenderUser(sess.getfirst());
        sess.addusermap(sess.getfirst(),newuse);

    }
    if(sess.getsecond() == "gen"){
        User *newuse = new GenreRecommenderUser(sess.getfirst());
        sess.addusermap(sess.getfirst(),newuse);
    }
    //cout << sess.getuserMap().size()<< endl;
}
std::string CreateUser::toString() const {
    return substring("CreateUser");
}

void ChangeActiveUser::act(Session &sess) {
    string userc = sess.getfirst();
    if (sess.getuserMap().count(userc) == 0) {
        error("there is no such user");
    } else {
        User *userTemp = sess.getuserMap()[userc];
        sess.changeactiveuser(userTemp);
        complete();
    }
}
string ChangeActiveUser::toString() const {
    return "basn";
}

void DeleteUser::act(Session &sess){
    string userToDel = sess.getfirst();
    if (sess.getuserMap().count(userToDel) == 0){
        error("There is no such user");
    } else {
        sess.deleteUser(userToDel);
        complete();
    }
}

string DeleteUser::toString() const {
    return "DeleteUser";
}

void DuplicateUser::act(Session &sess){

}

string DuplicateUser::toString() const {
    return "DuplicateUser";
}

std::string PrintWatchHistory::toString() const {

}

void Watch::act(Session &sess) {
    long id = std::stol(sess.getfirst());
    if(id>0 && id<=sess.getcontent().size()){
        cout <<sess.getcontent().at(id-1)->getname()<<endl;
        Watchable* watch = sess.getcontent().at(id-1);
      sess.getActiveUser().addtohistory(watch);
        complete();
    }
    else{
        error("no such content");
    }

}

std::string Watch::toString() const {
    return substring("Watch"); //change later
}

void PrintActionsLog::act(Session &sess) {
    cout<<sess.getactionsLog().size()<<endl;
    for(BaseAction* baseaction: sess.getactionsLog()){
        cout<<baseaction->toString()<<endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "blabla"; //change later
}

std::string BaseAction::substring(std::string action) const{
    if(status == ERROR){
        return action + " " + "ERROR: " + errorMsg;
    }
    else{
        return action + " " + "COMPLETED";
    }


}