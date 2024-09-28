#ifndef CPPCODE_MATCH_H
#define CPPCODE_MATCH_H

#include "Team.h"

class Match {

private:

    Team homeTeam;
    Team awayTeam;

public:
    Match(const Team &homeTeam,const Team &awayTeam);
    Team getHomeTeam() const;
    Team getAwayTeam() const;
};

#endif //CPPCODE_MATCH_H
