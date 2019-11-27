#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;

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
       if (abs(nextRec->getlength()-avg) > abs(s.getcontent().at(i)->getlength()-avg) ){
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
//    bool fit = true;
    while (tagCount.size() != 0) {
        maxTagName = findMaxName(tagCount);
        for (int i = 0; i < s.getcontent().size(); i++) {
            for (int j = 0; j < s.getcontent().at(i)->gettags().size() && !Watched; j++) {
                if (maxTagName == s.getcontent().at(i)->gettags().at(j)) {
                    nextRec = s.getcontent().at(i);
                    for (int k = 0; k < history.size() && !Watched; k++) {
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
            if (i != s.getcontent().size() && Watched) {
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