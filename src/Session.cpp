#include <iostream>
#include <fstream>
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/User.h"
#include "../include/Action.h"
//std::ifstream config1_file("../config1.json", std::ifstream::binary);
//config1_file >> content;

using namespace std;
class User;
class Watchable;
using json = nlohmann::json;

Session::Session(const std::string &_configFilePath) {
    std::ifstream i(_configFilePath);
    json j;
    i>>j;
    long id = 1;
    long defId = -2;
   json movies = j["movies"];
   json tvSeries = j["tv_series"];

    for (json& movie : movies) {
        Watchable* movie1 = new Movie(id, movie["name"], movie["length"] , movie["tags"]);
        content.push_back(movie1);
        id++;
    }
    for(json& series : tvSeries){
        json season = series["seasons"];
        for(int i = 0; i < season.size(); i++){
            for(int j = 1; j<= season[i]; j++){
                Episode* episode = new Episode(id, series["name"], series["episode_length"], i+1, j,series["tags"]);
                if(i== season.size()-1 && j==season[i]) {
                    episode->setNextEpisodeId(defId);
                }
                else{
                    episode->setNextEpisodeId(id);
                }
                content.push_back(episode);
                id++;
            }
        }
    }

    User *newUser = new LengthRecommenderUser("deafult");
    addusermap("deafult",newUser);
    activeUser = newUser;
}

User& Session::getActiveUser() const {
    return *activeUser;
}

std::string Session::getcommand() {
    return command;
}

std::string Session::getfirst() {
    return first;
}

std::string Session::getsecond() {
    return second;
}

std::unordered_map <std::string,User*> Session::getuserMap() {
    return userMap;
}

void Session::deleteUser(std::string userToDel){
    userMap.erase(userToDel);
}

//void Session::duplicateUser(std::string userToDup, User* newUser) {
//    newUser = userMap[userToDup];
//}

void Session::addusermap(std::string _name, User *_newuse) {
    userMap[_name] = _newuse;
}

std::vector<Watchable*>& Session::getcontent() {
    return content;
}

std::vector<BaseAction*>& Session::getactionsLog() {
    return actionsLog;
}

void Session::changeactiveuser(User* user)  {
    activeUser = user;
}

Session::~Session() {}
void Session::start() {
    printf("SPLFLIX is now on!"  "\n" );

    while(command != "exit"){
        printf("What would you like to do?" "\n");

        cin >> command;
        if(command == "createuser"){
            cin >> first;
            cin >> second;
            BaseAction *newuser = new CreateUser();
            newuser->act(*this);
            actionsLog.push_back(newuser);
        }
        else if(command == "changeuser"){
            cin >> first;
            BaseAction *changeuser = new ChangeActiveUser();
            changeuser->act(*this);
            actionsLog.push_back(changeuser);
        }

        else if(command == "watch"){
            cin >> first;
            BaseAction *watch = new Watch();
            watch->act(*this);
            actionsLog.push_back(watch);
            cout<< " continue watching? [y/n]" <<endl;
            cin >> second;
            if(second != "y" && second !="n"){
                cout << "Invalid Command" << endl;
            }
            while(second == "y"){
                first = std::to_string(content.at(std::stoi(first)-1)->getNextWatchable(*this)->getid());
                BaseAction *newWatch = new Watch();
                newWatch->act(*this);
                actionsLog.push_back(watch);
                cout<< " continue watching? [y/n]" <<endl;
                cin >> second;
                if(second != "y" && second !="n"){
                    cout << "Invalid Command" << endl;
                }
            }
        }
        else if(command == "log"){
            BaseAction *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        }
        else if(command == "deleteuser"){
            cin >> first;
            BaseAction *deleteuser = new DeleteUser();
            deleteuser->act(*this);
            actionsLog.push_back(deleteuser);
        }
        else if (command == "content"){
            BaseAction *content = new PrintContentList();
            content->act(*this);
            actionsLog.push_back(content);
        }
        else if (command == "watchhist"){
            BaseAction *watchlist = new PrintWatchHistory();
            watchlist->act(*this);
            actionsLog.push_back(watchlist);
        }
        else if(command=="dupuser"){
            cin >> first;
            cin >> second;
            BaseAction *duplicateUser = new DuplicateUser();
            duplicateUser->act(*this);
            actionsLog.push_back(duplicateUser);
        }
        else{
            cout<<"Illegal command"<<endl;
        }
    }
    cout <<"you enterd" << command; //stam hadpsa!@#!@#

}

