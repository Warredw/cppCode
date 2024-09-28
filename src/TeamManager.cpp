#include "TeamManager.h"


TeamManager::TeamManager() {
    std::vector<std::string> teamNames= {
            "Union", "Anderlecht", "Antwerp", "Club Brugge",
            "Cercle Brugge", "Genk", "Gent", "Mechelen",
            "STVV", "Standard", "Westerlo", "OHL",
            "Charleroi", "Eupen", "Kortrijk", "Molenbeek"
    };

    teams.resize((teamNames.size()));

    for(int i = 0; i < teamNames.size(); ++i) {
        teams[i] = Team(teamNames[i]);
    }
}

vector<Team>& TeamManager::getTeams() {
    return teams;
}

int TeamManager::getNumberTeams() {
    return teams.size();
}

int TeamManager::getNumberRounds() {
    return teams.size()-1;
}