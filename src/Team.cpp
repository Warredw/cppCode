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






