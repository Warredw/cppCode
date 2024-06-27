#ifndef CPPCODE_TEAM_H
#define CPPCODE_TEAM_H
#include "HapSet.h"


class Team {

public:
    int score;
    int goalsScored;
    int goalsReceived;
    string name;


    bool operator==(const Team& other) const {
        if(this->name == other.name) return true;
        return false;
    }

    explicit Team(const string &name);
    Team();
};
#endif //CPPCODE_TEAM_H
