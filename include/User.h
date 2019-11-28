#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual std::string getAlgoName() = 0;
    std::string getName() const;
    void  addtohistory(Watchable*);
    std::vector<Watchable*> get_history() const;
    virtual User* clone() = 0;
    void fixHistory (Session&);
    void copyHistory (User&);
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    std::string getAlgoName() ;
    virtual User* clone() ;
private:
    std::string algoName;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    std::string getAlgoName() ;
    virtual User* clone() ;
private:
    int lastRecId;
    std::string algoName;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    std::string getAlgoName() ;
    virtual User* clone() ;
private:
    std::string algoName;
};

#endif
