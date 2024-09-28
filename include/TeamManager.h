//
// Created by dewae on 26/09/2024.
//

#ifndef CPPCODE_TEAMMANAGER_H
#define CPPCODE_TEAMMANAGER_H

#include "Team.h"

class TeamManager {

private:
    vector<Team> teams;


public:
    TeamManager();
    vector<Team>& getTeams();
    int getNumberTeams();
    int getNumberRounds();
};


#endif //CPPCODE_TEAMMANAGER_H
