#ifndef CPPCODE_MATCH_H
#define CPPCODE_MATCH_H
#include "Team.h"

class Match {

public:
    Team* homeTeam;
    Team* awayTeam;
    Match(Team &homeTeam, Team &awayTeam);
};


#endif //CPPCODE_MATCH_H
