#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
//#include <bits/valarray_before.h>
#include "Action.h"
using namespace std;
class User;
class Watchable;
class Movie;
class Episode;
class Session{
public:
    Session(const std::string &configFilePath);
    ~Session(); // Destructor
    Session(const Session&); // Copy Constructor
    void start();
    User& getActiveUser() const;
    std::string getfirst();
    std::string getsecond();
    const unordered_map<std::string,User*> getuserMap() const;
    void deleteUser(std::string);
    void addusermap(std::string,User*);
    const vector<Watchable*>& getcontent() const;
    const vector<BaseAction*>& getactionsLog() const;
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
