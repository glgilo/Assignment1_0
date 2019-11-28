#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/User.h"
#include "../include/Action.h"

using namespace std;
class User;
class Watchable;
using json = nlohmann::json;

Session::~Session() { // Destructor
    clean();
}

void Session::clean(){
    for (Watchable* cont: content)
        delete(cont);
    content.clear();
    for (BaseAction* action: actionsLog)
        delete(action);
    actionsLog.clear();
    for (pair<string,User*> user: userMap)
        delete(user.second);
    userMap.clear();
}

Session::Session(const Session& other) {  // Copy Constructor
         copy(other);
 }

 Session& Session::operator=(const Session &other){ // Copy assignment operator
    if(this == &other){
        return *this;
    }
    clean();
    copy(other);
     return *this;
}

Session::Session(Session&& other) { //Move Constructor
    move(other);
}

Session& Session::operator=(const Session &&other) { //Move assignment operator
    if(this == &other){
        return *this;
    }
    clean();
    move(other);
    return *this;
}

void Session::move(const Session &other) {
    for (Watchable *cont: other.getcontent()) {
        content.push_back(cont);
        cont = nullptr;
    }
    for (BaseAction* action: other.getactionsLog()) {
        actionsLog.push_back(action);
        action = nullptr;
    }
    for (pair<string,User*> user: other.getuserMap()) {
        userMap.insert(user);
        user.second = nullptr;
    }
    activeUser = userMap[other.getActiveUser().getName()];
}

void Session::copy(const Session &other) {
    for (int i = 0; i < other.getcontent().size(); i++)
        content.push_back(other.getcontent().at(i)->clone());
    for (pair<string,User*> user: other.getuserMap()) {
        User* temp = user.second->clone();
        temp->fixHistory(*this);
        userMap.insert({user.first, temp});
    }
    for (BaseAction* action: other.getactionsLog())
        actionsLog.push_back(action->clone());
    activeUser = userMap[other.getActiveUser().getName()];
}

Session::Session(const std::string &_configFilePath) {
    std::ifstream i(_configFilePath);
    json j;
    i>>j;
    long id = 1;
    long defId = -2;
   json movies = j["movies"];
   json tvSeries = j["tv_series"];

    for (json& movie : movies) {
        auto* movie1 = new Movie(id, movie["name"], movie["length"] , movie["tags"]);
        content.push_back(movie1);
        id++;
    }
    for(json& series : tvSeries){
        json season = series["seasons"];
        for(int i = 0; i < season.size(); i++){
            for(int j = 1; j<= season[i]; j++){
                auto* episode = new Episode(id, series["name"], series["episode_length"], i+1, j,series["tags"]);
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
    User *newUser = new LengthRecommenderUser("default");
    addusermap("default",newUser);
    activeUser = newUser;
}

User& Session::getActiveUser() const {
    return *activeUser;
}


std::string Session::getfirst() {
    return first;
}

std::string Session::getsecond() {
    return second;
}

const unordered_map <std::string,User*> Session::getuserMap() const {
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

const vector<Watchable*>& Session::getcontent() const {
    return content;
}

const vector<BaseAction*>& Session::getactionsLog() const {
    return actionsLog;
}

void Session::changeactiveuser(User* user)  {
    activeUser = user;
}

//Session::~Session() {}
void Session::start() {
    printf("SPLFLIX is now on!"  "\n" );

    while(true){
        printf("What would you like to do?" "\n");
        std::string inputLine;
        getline(std::cin,inputLine);
        std::istringstream iss(inputLine);
        std::getline(iss,command,' ');
        std::getline(iss,first,' ');
        std::getline(iss,second,' ');
//        cin >> command;
        if(command == "createuser"){
//            cin >> first;
//            cin >> second;
            BaseAction *newuser = new CreateUser();
            newuser->act(*this);
            actionsLog.push_back(newuser);
        }
        else if(command == "changeuser"){
//            cin >> first;
            BaseAction *changeuser = new ChangeActiveUser();
            changeuser->act(*this);
            actionsLog.push_back(changeuser);
        }

        else if(command == "watch"){
//            cin >> first;
            BaseAction *watch = new Watch();
            watch->act(*this);
            actionsLog.push_back(watch);
            cout<< " continue watching? [y/n]" <<endl;
            cin >> second;
            if(second != "y" && second !="n"){
                cout << "Invalid Command" << endl;
            }
            while(second == "y") {
                std::getline(iss, second, ' ');
                first = std::to_string(content.at(std::stoi(first) - 1)->getNextWatchable(*this)->getid());
                BaseAction *newWatch = new Watch();
                newWatch->act(*this);
                actionsLog.push_back(newWatch);
                if (content.at(std::stoi(first) - 1)->getNextWatchable(*this) == nullptr)
                    break;
                cout<< ", continue watching? [y/n]" <<endl;
                cin >> second;
                if(second != "y" && second !="n"){
                    cout << "Invalid Command" << endl;
                }
            }
            getline(std::cin,inputLine);
            std::istringstream iss(inputLine);
        }
        else if(command == "log"){
            BaseAction *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        }
        else if(command == "deleteuser"){
//            cin >> first;
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
//            cin >> first;
//            cin >> second;
            BaseAction *duplicateUser = new DuplicateUser();
            duplicateUser->act(*this);
            actionsLog.push_back(duplicateUser);
        }
        else if (command == "exit"){
            BaseAction *exit = new Exit();
            exit->act(*this);
            actionsLog.push_back(exit);
            break;
        }
        else{
            cout<<"Illegal command"<<endl;
        }
    }
//    cout <<"you enterd" << command; //stam hadpsa!@#!@#
}

