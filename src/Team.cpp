#include "Team.h"

Team::Team(const string &name) {
    this-> name = name;
    goalsReceived = 0;
    goalsScored = 0;
    score = 0;
}


Team::Team() {
    this->name = "empty";
}

const string &Team::getName() {
    return name;
}

const int& Team::getGoalsScored() const{
    return goalsScored;
}

const int& Team::getGoalsReceived() const{
    return goalsReceived;
}

const int& Team::getScore() const {
    return score;
}



