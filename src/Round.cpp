#include "Round.h"
#include <stdexcept>

void Round::addMatch(const Match &match) {
    matches.push_back(match);
}

const Match& Round::getMatch(int index) const {
    if(index < 0 || index > matches.size()) {
        throw std::out_of_range("Index is out of range");
    }
    return matches[index];
}

vector<Match> Round::getMatches() const{
    return matches;
}

