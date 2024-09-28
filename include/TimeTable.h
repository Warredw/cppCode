//
// Created by dewae on 26/09/2024.
//

#ifndef CPPCODE_TIMETABLE_H
#define CPPCODE_TIMETABLE_H
#include "Round.h"
#include "TeamManager.h"
#include <filesystem>

class TimeTable {

private:
    vector<Round> rounds;
    vector<vector<vector<int>>> homeAwayGames;
    std::shared_ptr<TeamManager> teamManager;

public:
    explicit TimeTable(std::shared_ptr<TeamManager> teamManager);
    vector<Round> getRounds () const;
    void resizeRounds(int newSize);
    void resizeHomeAwayGames(const std::shared_ptr<TeamManager>& teamManager);
    void addMatchToRound(int round, Match match);
    void setHomeAwayGamesValue(int i, int j, int z, int number);
    int getHomeAwayGamesValue (int i, int j, int z) const;

};

#endif //CPPCODE_TIMETABLE_H
