#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() const = 0;
    void setAction(BaseAction*) const;
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
    std::string substring(std::string) const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};

class DeleteUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};


class DuplicateUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};

class PrintContentList : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};

class PrintWatchHistory : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};


class Watch : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};


class PrintActionsLog : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};

class Exit : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
};
#endif
