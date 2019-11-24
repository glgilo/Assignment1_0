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
    cout << sess.getuserMap().size()<< endl;
    if(sess.getsecond() != "len" && sess.getsecond() != "rer" && sess.getsecond() != "gen"){
        error("NOT valid algorithm");
    }
    else if(sess.getuserMap().count(sess.getfirst()) != 0){
        error("the new user name is already taken");
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

}
std::string CreateUser::toString() const {
    return substring("CreateUser");
}

void ChangeActiveUser::act(Session &sess) {
    std::string userc = sess.getfirst();
    if(sess.getuserMap().count(userc) == 0){
        error("there is no such user");
    } else{

        sess.changeactiveuser(sess.getActiveUser());//ajkogjoagojadfgnho@#$#$%$^$
    }
    sess.getActiveUser() = sess.getuserMap().
}

std::string PrintWatchHistory::toString() const {

}

void Watch::act(Session &sess) {
    long id = std::stol(sess.getfirst());
    if(id>0 && id<=sess.getcontent().size()){
        cout <<sess.getcontent().at(id-1)->toString()<<endl;
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