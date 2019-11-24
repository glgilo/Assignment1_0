#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
//#include <bits/valarray_before.h>
#include "Action.h"

class User;
class Watchable;
class Movie;
class Episode;
class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    User& getActiveUser() const;
    std::string getcommand();
    std::string getfirst();
    std::string getsecond();
    std::unordered_map<std::string,User*> getuserMap();
    void addusermap(std::string,User*);
    std::vector<Watchable*>& getcontent();
    std::vector<BaseAction*>& getactionsLog();
    void changeactiveuser(User*);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string command;
    std::string first;
    std::string second;
};
#endif
