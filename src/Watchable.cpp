#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

////Watchable Constructor implementation
Watchable::Watchable(long id, int length, const vector<string> &tags):id(id),length(length),tags(tags),type(){}

////Watchable Destructor
Watchable::~Watchable() {
    tags.clear();
}


////get id method
long Watchable::get_id() const{

    return id;
}
////get length method
int Watchable::get_length() const{
    return length;
}

////get Tags Vector method
std::vector<std::string> Watchable::get_tags() const{
    return tags;
}

///set Type
void Watchable::set_Type(bool t) {
    type = t;
}

////Movie class ------------------------------------------------------------------------------------------
////Movie constructor
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags): Watchable(id,length,tags),name(name){
    set_Type(false);
}


////Movie toString
string Movie::toString() const {
    vector<string> vec = get_tags();
    std::ostringstream vts;
    if (!vec.empty())
    {
        std::copy(vec.begin(), vec.end()-1,
                  std::ostream_iterator<string>(vts, ", "));
        vts << vec.back();
    }
    string tagsString = vts.str();
    string output = std::to_string(get_id()) + ". " + name + " " + std::to_string(get_length()) + " minutes [" + tagsString + "]";
 return output ;
};

///printString
string Movie::printString() const {
    return std::to_string(get_id()) + ". " + name;
}

//// Movie getNextWatchable
Watchable* Movie::getNextWatchable(Session &) const {
    return nullptr;
}

///Movie clone
Watchable* Movie::clone() {
   return new Movie(*this);
}


////-------------------------------------------------------------------------------------------------

////Episode class -----------------------------------------------------------------------------------
////Episode constructor
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags): Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId() {
    set_Type(true);

}

////Episode toString
string Episode::toString() const {
    vector<string> vec = get_tags();
    std::ostringstream vts;
    if (!vec.empty())
    {
        std::copy(vec.begin(), vec.end()-1,
                  std::ostream_iterator<string>(vts, ", "));
        vts << vec.back();
    }
    string tagsString = vts.str();
    string output = std::to_string(get_id()) + ". " + seriesName + " S" + std::to_string(season) + "E" + std::to_string(episode)  + " " +  std::to_string(get_length()) + " minutes [" + tagsString + "]";
    return output ;
}

///Episode printString
string Episode::printString() const {
    return std::to_string(get_id()) + ". " + seriesName + " S" + std::to_string(season) + "E" + std::to_string(episode);
}


///Episode getNextWatchable
Watchable* Episode::getNextWatchable(Session & sess) const {
    Watchable* output = nullptr;
    long currNextId = this->get_nextEpisodeId();
    if(currNextId != 0){
        output = sess.get_content()[currNextId-1];
    }
    return output;
}

///set nextEpisodeId
void Episode::set_nextEpisodeId(int id) {
    this->nextEpisodeId = id;
}

///get_nextEpisodeId
long Episode::get_nextEpisodeId() const {
    //return this->nextEpisodeId;
    return nextEpisodeId;
}

///Episode clone
Watchable* Episode::clone() {

    return new Episode(*this);

}


////-----------------------------------------------------------------------------------------------------