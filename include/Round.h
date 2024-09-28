//
// Created by dewae on 23/09/2024.
//

#ifndef CPPCODE_ROUND_H
#define CPPCODE_ROUND_H

#include "Match.h"

class Round {

private: vector<Match> matches;

public:

    vector<Match> getMatches() const;
    void addMatch(const Match& match);
    const Match& getMatch(int index) const;
};

#endif //CPPCODE_ROUND_H
