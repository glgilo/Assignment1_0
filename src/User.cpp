#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;

User::~User() {};

User::User(const std::string &_name): history(), name(_name) {}
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

LengthRecommenderUser::LengthRecommenderUser(const std::string &_name): User (_name), algoName("len")  {
    //algoName = "len";
}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    float avg = 0;
    int maxLength = 0;
    for(Watchable* hisCont: history){
        avg = avg + (float)hisCont->getlength();
    }
    for(Watchable* cont: s.getcontent()){
        if (cont->getlength() >= maxLength)
            maxLength = cont->getlength();
    }
    avg = avg/history.size();
    bool seen = false;
    std::string defaultName = "";
    std::vector<std::string> defaultTag;
    Watchable* temp = new Movie(-1, defaultName ,maxLength*2, defaultTag);
    Watchable* nextRec = temp;
    for(int i = 1;(unsigned)i < s.getcontent().size(); i++){
       if (abs((float)nextRec->getlength()-avg) > abs((float)s.getcontent().at(i)->getlength()-avg) ){
           for(int j = 0;(unsigned) j <  history.size() && !seen; j++) {
               if(s.getcontent().at(i) == history.at(j))
                   seen = true;
           }
           if (!seen)
                nextRec = s.getcontent().at(i);
       }
       seen = false;
   }
    delete(temp);
    if (nextRec->getname() == defaultName)
        return nullptr;
    return nextRec;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &_name): User(_name), lastRecId(0), algoName("rer") {
//    lastRecId = 0;
//    algoName = "rer";
}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    //lastRecId = (lastRecId + 1) % (long)history.size();
    lastRecId = (history.at(history.size()-1)->getid() + 1) % (long)history.size();
    Watchable* nextRec = history.at(lastRecId);
    return nextRec;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &_name): User(_name), algoName("gen") {
//    algoName = "gen";
}

std::string findMaxName(std::unordered_map<std::string,int> &tagMax) {
    std::string maxTagName = "";
    int maxTagCount = 0;
    for (auto &tag: tagMax) {
        if (tag.second > maxTagCount) {
                maxTagName = tag.first;
                maxTagCount = tag.second;
        }
        else if(tag.second == maxTagCount && maxTagName > tag.first ){
                maxTagName = tag.first;
                maxTagCount = tag.second;
        }
    }
    return maxTagName;
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    std::unordered_map<std::string,int> tagCount;
    for(Watchable* content: history){
        for(std::string& tag : content->gettags()){
            if(tagCount.count(tag) == 0){
                tagCount.insert({tag,1});
            } else{
                tagCount[tag]++;
            }
        }
    }
    std::string maxTagName = "";
    int maxTagCount = 0;
    for (auto& tag: tagCount){
        if (tag.second > maxTagCount){
            maxTagName = tag.first;
            maxTagCount = tag.second;
        }

    }
    Watchable* nextRec;
    bool Watched = false;
    while (!tagCount.empty()) {
        maxTagName = findMaxName(tagCount);
        for (int i = 0;(unsigned)i < s.getcontent().size(); i++) {
            for (int j = 0;(unsigned) j < s.getcontent().at(i)->gettags().size() && !Watched; j++) {
                if (maxTagName == s.getcontent().at(i)->gettags().at(j)) {
                    nextRec = s.getcontent().at(i);
                    for (int k = 0;(unsigned) k < history.size() && !Watched; k++) {
                        if (s.getcontent().at(i) == history.at(k)) {
                            Watched = true;
                            nextRec = nullptr;
                        }
                    }
                    if (!Watched) {
                        return nextRec;
                    }
                }
            }
            if ((unsigned)i != s.getcontent().size() && Watched) {
                Watched = false;
            }
        }
        tagCount.erase(maxTagName);
    }
    return nullptr;
}

void User::addtohistory(Watchable* watch) {
    history.push_back(watch);
}


User* LengthRecommenderUser::clone() {
    auto* userClone = new LengthRecommenderUser(getName());
    copyHistory(*userClone);
    userClone->algoName = algoName;
    return userClone;
}

User* RerunRecommenderUser::clone() {
    auto* userClone = new RerunRecommenderUser(getName());
    userClone->lastRecId = lastRecId;
    userClone->algoName = algoName;
    copyHistory(*userClone);
    return userClone;
}

User* GenreRecommenderUser::clone() {
    auto* userClone = new GenreRecommenderUser(getName());
    userClone->algoName = algoName;
    copyHistory(*userClone);
    return userClone;
}

void User::fixHistory(Session &sess) {
    long id = 0;
    for(int i = 0;(unsigned)i < this->get_history().size(); i++){
        id = this->get_history().at(i)->getid();
        history.at(i) = sess.getcontent().at(id - 1);
    }
}

void User::copyHistory(User& other) {
    for (int i = 0; (unsigned) i < this->get_history().size(); i++){
        other.addtohistory(this->get_history().at(i));
    }
}
