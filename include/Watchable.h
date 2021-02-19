#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual string printString() const = 0;
    long get_id() const;
    int get_length() const;
    std::vector<std::string> get_tags() const;
    void set_Type(bool type);
    virtual Watchable* clone() = 0;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
    bool type; // Movie = false ; Episode = True;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual string printString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Watchable* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual string printString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void set_nextEpisodeId(int id);
    long get_nextEpisodeId() const;
    virtual Watchable* clone();

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif