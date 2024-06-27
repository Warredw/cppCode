#ifndef CPPCODE_TIMETABLE_H
#define CPPCODE_TIMETABLE_H
#include "HapSet.h"
#include "Match.h"

class TimeTable {

public:
    // storing where the home games occur in the first half of the tournament
    vector<vector<vector<int>>> homeAwayGames;

    int numberSchedulesPossible;

    HapSet hapSet;
    Team* teams;

    struct Round{
        vector<Match> round;
    };

    struct Schedule{
        vector<Round> schedule;
    };

    Schedule timeTableFirstHalf;
    vector<Schedule> allTimeTablesSecondHalf;

    TimeTable();
    ~TimeTable();

    void makeScheduleFirstHalf();
    void setTeams() const;
    void makeAllSchedulesSecondHalf();
};
#endif //CPPCODE_TIMETABLE_H
