#include "../include/Watchable.h"
#include <string>

//Watchable::Watchable(): id(-1), length(-1),tags() {}
Watchable::Watchable(long _id, int __length, const std::vector<std::string> &_tags): id(_id), length(__length), tags(_tags) {}

Watchable::~Watchable() {}

Movie::Movie(long _id, const std::string &_name, int _length, const std::vector<std::string> &_tags): Watchable(_id, _length, _tags), name(_name) {}
std::string Movie::toString() const {
    std::string outputtags = "" ;
    for(std::string tag1 : gettags()){
        outputtags = outputtags + tag1;
    }

    return  std::to_string(getid()) + ". " + name  + " " + std::to_string(getlength()) + " " + "[" + outputtags + "]";

}
Watchable* Movie::getNextWatchable(Session &) const {
    return nullptr; //change later
}

std::string Movie::getname() const {
    return name;
}

std::string Episode::getname() const {
    return seriesName + " "  + "S" + std::to_string(season) + "E" +std::to_string(episode) ;
}

Episode::Episode(long _id, const std::string &_seriesName, int _length, int _season, int _episode, const std::vector<std::string> &_tags):  Watchable(_id, _length, _tags), season(_season), episode(_episode), seriesName(_seriesName) {}
std::string Episode::toString() const {
    std::string outputtags = "";
    for(std::string tag1 : gettags()){
//        if(tag1 == gettags().at(gettags().size()-1)){
//            outputtags = outputtags + tag1;
//        }
        outputtags = outputtags + tag1 +",";
    }
    return std::to_string(getid())+ ". " + seriesName + " " + "S" + std::to_string(season) + "E" + std::to_string(episode) + " " +std::to_string(getlength()) + " minutes" + " [" + outputtags.substr(0,outputtags.size()-1) + "]";
}
Watchable* Episode::getNextWatchable(Session &) const {
    return nullptr; //change later
}


int Watchable::getlength() const{
    return length;
}

 long Watchable::getid() const {
    return id;
}
std::vector<std::string> Watchable::gettags() const{
    return tags;
}

