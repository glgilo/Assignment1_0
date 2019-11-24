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
    int count = 0;
    int sum = 0;
    int avg = 0;
    for  (Watchable* watch : history){
        sum = sum + watch->getlength();
        count++;
    }
    avg = sum/count;
    std::vector<Watchable*> notseen;

    return nullptr; //change later
}

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