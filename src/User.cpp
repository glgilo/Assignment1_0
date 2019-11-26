#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <iostream>
#include <algorithm>

User::User(const std::string &_name): name(_name) {}
std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history() const{
    return history;
}

std::string LengthRecommenderUser::getAlgoName() {
    return algoName;
}

std::string RerunRecommenderUser::getAlgoName() {
    return algoName;
}

std::string GenreRecommenderUser::getAlgoName() {
    return algoName;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string &_name): User (_name) {
    count = 0;
    avg = 0;
    algoName = "len";
}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
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
}

//void


RerunRecommenderUser::RerunRecommenderUser(const std::string &_name): User(_name) {
    lastRecId = 0;
    algoName = "rer";
}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    lastRecId = (lastRecId + 1) % history.size();
    Watchable* nextRec = history.at(lastRecId);
    //std::cout<<history.size()<<std::endl;
    return nextRec;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &_name): User(_name) {
    algoName = "gen";
}
Watchable* GenreRecommenderUser::getRecommendation(Session &_s) {

    return nullptr;//change later
}

void User::addtohistory(Watchable* watch) {
    history.push_back(watch);
}