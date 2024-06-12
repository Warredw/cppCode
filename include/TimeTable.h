#ifndef CPPCODE_TIMETABLE_H
#define CPPCODE_TIMETABLE_H
#include "HapSet.h"
#include "Match.h"


class TimeTable {

public:

    HapSet hapSet;
    Team* teams;

    // Length of the vector is the number of rounds, in each round there are 8 matches
    vector<vector<Match>> timeTableFirstHalf;
    vector<vector<Match>> timeTableSecondHalf;

    TimeTable();
    ~TimeTable();

    void makeScheduleFirstHalf();
    void makeScheduleSecondHalf();
    void setTeams() const;

};


#endif //CPPCODE_TIMETABLE_H
