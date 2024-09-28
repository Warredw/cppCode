#ifndef CPPCODE_TEAM_H
#define CPPCODE_TEAM_H
#include "HapSet.h"


class Team {

private:
    int score;
    int goalsScored;
    int goalsReceived;
    string name;



public:

    bool operator==(const Team& other) const {
        if(this->name == other.name) return true;
        return false;
    }
    explicit Team(const string &name);
    Team();

    const int& getScore() const;
    const int& getGoalsScored() const;
    const int& getGoalsReceived() const;
    const string& getName();

};

#endif //CPPCODE_TEAM_H



