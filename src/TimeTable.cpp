//
// Created by dewae on 26/09/2024.
//

#include "TimeTable.h"


vector<Round> TimeTable::getRounds() const{
    return rounds;
}

TimeTable::TimeTable(std::shared_ptr<TeamManager> teamManager) : teamManager(teamManager) {};

void TimeTable::resizeRounds(int newSize) {
rounds.resize(newSize);
}

void TimeTable::resizeHomeAwayGames(const std::shared_ptr<TeamManager>& teamManager) {
    homeAwayGames.resize(teamManager->getNumberTeams());
    for(int i = 0; i < teamManager->getNumberTeams(); i++) {
        homeAwayGames[i].resize(teamManager->getNumberTeams());
        for(int j = 0; j < teamManager->getNumberTeams(); j++) {
            homeAwayGames[i][j].resize(teamManager->getNumberRounds());

            for(int k = 0;  k < teamManager->getNumberRounds(); k++) {
                homeAwayGames[i][j][k] = 0;
            }
        }
    }
}

void TimeTable::addMatchToRound(int round, Match match) {
    rounds.at(round).addMatch(match);
}


void TimeTable::setHomeAwayGamesValue(int i, int j, int z, int number) {
    homeAwayGames[i][j][z] = number;
}

int TimeTable::getHomeAwayGamesValue(int i, int j, int z) const{
    return homeAwayGames[i][j][z];
}




    


