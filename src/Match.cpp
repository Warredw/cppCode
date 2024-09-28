#include "Match.h"

Match::Match(const Team& homeTeam,const  Team& awayTeam)
    :homeTeam(homeTeam), awayTeam(awayTeam) {
}


Team Match::getHomeTeam() const {
    return homeTeam;
}

Team Match::getAwayTeam() const {
    return awayTeam;
}



