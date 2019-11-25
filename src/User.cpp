#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

User::User(const std::string &_name): name(_name) {}
std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history() const{
    return history;
}
LengthRecommenderUser::LengthRecommenderUser(const std::string &_name): User (_name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    User& user = s.getActiveUser();
    std::vector<Watchable*> history = user.get_history();
    avg = ((avg*count)+history.at(history.size()-1)->getlength())/(count+1);
    count++;
    bool seen = false;
    std::string defaultName = "";
    std::vector<std::string> defaultTag;
    Watchable* temp = new Movie(-1, defaultName ,INTMAX_MAX, defaultTag);
    Watchable* nextRec = temp;
    for(int i = 1; i < s.getcontent().size(); i++){
       if (abs(nextRec->getlength()-avg) >= abs(s.getcontent().at(i)->getlength()-avg) ){
           for(int j = 0; j < history.size() && !seen; j++) {
               if(s.getcontent().at(i) == history.at(j))
                   seen = true;
           }
           if (!seen)
                nextRec = s.getcontent().at(i);
       }
       seen = false;
   }
    delete(temp);
    return nextRec;
    //eturn s.getcontent().at(10);
}

//void


RerunRecommenderUser::RerunRecommenderUser(const std::string &_name): User(_name) {}
Watchable* RerunRecommenderUser::getRecommendation(Session &_s) {
    return nullptr;//change later
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &_name): User(_name) {}
Watchable* GenreRecommenderUser::getRecommendation(Session &_s) {
    return nullptr;//change later
}

void User::addtohistory(Watchable* watch) {
    history.push_back(watch);
}