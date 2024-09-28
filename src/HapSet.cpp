#include "HapSet.h"

HapSet::HapSet(const string& dNotation, const string& type) {
    this->type = type; 
    hapSetFirstHalf = nullptr;
    hapSetSecondHalf = nullptr;
    this->dNotation = dNotation;
    nrTeams = 16;
    makeHapSetFirstHalf();
    makeHapSetSecondHalf();
}


HapSet::~HapSet() {

    for(int i = 0; i < nrTeams; i++) {
        delete[] hapSetFirstHalf[i]; 
        delete[] hapSetSecondHalf[i];
    }
    delete[] hapSetFirstHalf; 
    delete[] hapSetSecondHalf; 
}

int** HapSet::getHapSetFirstHalf() const {
    return hapSetFirstHalf;
}

int** HapSet::getHapSetSecondHalf() const {
    return hapSetSecondHalf;
}

void HapSet::makeHapSetFirstHalf() {

    // Initialize a HAP-set of the correct size
    hapSetFirstHalf = new int*[nrTeams];
    for(int i = 0; i < nrTeams; i++) {
        hapSetFirstHalf[i] = new int[nrTeams-1];
    }
    // Assume the complementary HAPs with a break in round 1 always to be present
    // Give team 1 a home-break in round 1 and team 2 an away-break in round 1
    hapSetFirstHalf[0][0] = 1;
    hapSetFirstHalf[0][nrTeams-2] = 1;
    hapSetFirstHalf[1][0] = 0;
    hapSetFirstHalf[1][nrTeams-2] = 0;

    // Fill the other rounds for team 1
    for(int j = 1; j < nrTeams -2; j++) {
        if (j%2 == 0) hapSetFirstHalf[0][j] = 1;
        else hapSetFirstHalf[0][j] = 0;
    }

    // Fill the other rounds for team 2
    for(int j = 1; j < nrTeams -2; j++) {
        if (j%2 == 0) hapSetFirstHalf[1][j] = 0;
        else hapSetFirstHalf[1][j] = 1;
    }

    // Initialize a vector to store rounds where a break will occur (after the one in round 1)
    vector<int> roundsWithBreak;

    // Go over the dNotation to get the break rounds and store them in the vector
    for (int round = 0, i = 0; i < dNotation.size() - 1; i++) {
        int value = dNotation[i] - '0';
        round += value;
        roundsWithBreak.push_back(round);
    }

    // Add the breaks to the HAP-set
    for(int nextBreakTeam = 2, i = 0; i < roundsWithBreak.size(); i++) {
        int breakRound = roundsWithBreak[i];
        hapSetFirstHalf[nextBreakTeam][breakRound] = 1;
        hapSetFirstHalf[nextBreakTeam][breakRound-1] = 1;
        hapSetFirstHalf[nextBreakTeam+1][breakRound] = 0;
        hapSetFirstHalf[nextBreakTeam+1][breakRound-1] = 0;
        nextBreakTeam += 2;
    }

    // Fill all the non-break rounds
    // Go over every team (except the first 2)
    for(int i = 2; i < nrTeams; i++) {

        // get the break round
        int breakRound = 0;
        for(int j = 0; j < nrTeams-1; j++) {
            if (hapSetFirstHalf[i][j] == 1 || hapSetFirstHalf[i][j] == 0) {
                breakRound = j + 1;
                break;
            }
        }

        // Fill the rounds after the break
        for(int j = breakRound + 1; j < nrTeams -1; j++) {
            if (hapSetFirstHalf[i][j-1] == 0) hapSetFirstHalf[i][j] = 1;
            else hapSetFirstHalf[i][j] = 0;
        }

        // Fill the rounds before the break
        for(int j = breakRound - 2; j >= 0; j--){
            if (hapSetFirstHalf[i][j+1] == 0) hapSetFirstHalf[i][j] = 1;
            else hapSetFirstHalf[i][j] = 0;
        }
    }

    // save the HAP-set as a txt file
    std::ofstream file("C:/Users/dewae/Documents/thesis/cppCode/output/hapSetFirstHalf/hapSetFirstHalf.txt");
    for(int i = 0; i < nrTeams; i++) {
        for(int j = 0; j < nrTeams-1; j++) {
            file << hapSetFirstHalf[i][j];
        }
        file<<std::endl;
    }
    file.close();
}
// Mirror the HAP-set to get the HAP-set of the second half
void HapSet::makeHapSetSecondHalf() {

    // Initialize size HAP-set second half
        hapSetSecondHalf = new int*[nrTeams];
        for (int i = 0; i < nrTeams; i++) {
            hapSetSecondHalf[i] = new int[nrTeams - 1];
        }

    if(type == "mirrored") {
        // Go over every value of the first half and take the reverse
        for(int i = 0; i < nrTeams; i++) {
            for(int j = 0; j < nrTeams-1; j++) {
                if(hapSetFirstHalf[i][j] == 1) hapSetSecondHalf[i][j] = 0;
                else hapSetSecondHalf[i][j] = 1;
            }
        }
    }

    else if (type == "inverted") {

        for(int i = 0; i < nrTeams; i++) {
            for(int j = nrTeams-2; j >= 0; j-- ) {
                if(hapSetFirstHalf[i][j] == 1) hapSetSecondHalf[i][nrTeams-2 - j ] = 0; 
                else hapSetSecondHalf[i][nrTeams-2-j] = 1;
            }
        }
        
    }
}



